#include "Core/ZoneManager.h"
#include "Core/WaveManager.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"

AZoneManager::AZoneManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AZoneManager::BeginPlay()
{
    Super::BeginPlay();

    // Start with a black screen.
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->PlayerCameraManager->StartCameraFade(0.0f, 1.0f, 0.0f, FadeColor, false, true);
    }

    StartCurrentZone();

    PlayFadeInAndNotify();
}

void AZoneManager::StartCurrentZone()
{
    if (!ZoneWaveManagers.IsValidIndex(CurrentZoneIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("ZoneManager: No WaveManager assigned for zone %d."), CurrentZoneIndex);
        return;
    }

    bWaitingForRest = false;
    TimeRemaining = DayDuration;

    // Listen for the active WaveManager's completion event.
    AWaveManager* CurrentWaveManager = ZoneWaveManagers[CurrentZoneIndex];
    if (CurrentWaveManager)
    {
        CurrentWaveManager->OnWaveCompleted.AddDynamic(this, &AZoneManager::OnZoneWaveCompleted);
    }

    UE_LOG(LogTemp, Warning, TEXT("ZoneManager: Zone %d started. Day duration: %f."), CurrentZoneIndex, DayDuration);

    GetWorldTimerManager().SetTimer(ZoneTimerHandle, this, &AZoneManager::UpdateZoneTimer, 1.0f, true);
}

void AZoneManager::UpdateZoneTimer()
{
    TimeRemaining -= 1.0f;
    OnZoneTimerUpdated.Broadcast(TimeRemaining);

    if (TimeRemaining <= 0.0f)
    {
        GetWorldTimerManager().ClearTimer(ZoneTimerHandle);

        UE_LOG(LogTemp, Warning, TEXT("ZoneManager: Zone %d day phase ended. Starting wave."), CurrentZoneIndex);

        if (ZoneWaveManagers.IsValidIndex(CurrentZoneIndex))
        {
            ZoneWaveManagers[CurrentZoneIndex]->StartWave();
        }
    }
}

void AZoneManager::OnZoneWaveCompleted(int32 CompletedDay)
{
    UE_LOG(LogTemp, Warning, TEXT("ZoneManager: Zone %d wave completed. Day: %d."), CurrentZoneIndex, CompletedDay);

    bWaitingForRest = true;

    UE_LOG(LogTemp, Warning, TEXT("ZoneManager: Zone %d cleared. Rest point available."), CurrentZoneIndex);

    OnRestPointAvailable.Broadcast();
}

void AZoneManager::OnPlayerRested()
{
    if (!bWaitingForRest) return;

    UE_LOG(LogTemp, Warning, TEXT("ZoneManager: Player rested. Waiting state: %d."), bWaitingForRest);

    if (ZoneWaveManagers.IsValidIndex(CurrentZoneIndex) && ZoneWaveManagers[CurrentZoneIndex])
    {
        ZoneWaveManagers[CurrentZoneIndex]->StopWaveManager();
        ZoneWaveManagers[CurrentZoneIndex]->ClearDeadZombies();
    }

    bWaitingForRest = false;

    // Keep the screen black until the next zone is initialized.
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->PlayerCameraManager->StartCameraFade(
            0.0f, 1.0f,
            FadeOutDuration,
            FadeColor,
            false,
            true
        );
    }

    GetWorldTimerManager().SetTimer(FadeTransitionTimerHandle, this, &AZoneManager::DoZoneTransition, FadeOutDuration, false);
}

void AZoneManager::DoZoneTransition()
{
    CurrentZoneIndex++;

    UE_LOG(LogTemp, Warning, TEXT("ZoneManager: Moving to zone %d."), CurrentZoneIndex);

    OnZoneChanged.Broadcast(CurrentZoneIndex);

    if (ZoneWaveManagers.IsValidIndex(CurrentZoneIndex))
    {
        StartCurrentZone();
        PlayFadeInAndNotify();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ZoneManager: All zones completed. Final sequence should start here."));
    }
}

void AZoneManager::PlayFadeInAndNotify()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, FadeInDuration, FadeColor, false, false);
    }

    GetWorldTimerManager().SetTimer(
        FadeInCompletedTimerHandle,
        this,
        &AZoneManager::OnFadeInCompleted,
        FadeInDuration,
        false
    );
}

void AZoneManager::OnFadeInCompleted()
{
    int32 DayNumber = CurrentZoneIndex + 1;

    UE_LOG(LogTemp, Warning, TEXT("ZoneManager: Fade-in completed. Displaying Day %d."), DayNumber);

    OnZoneTransitionVisualsReady.Broadcast(DayNumber);
}