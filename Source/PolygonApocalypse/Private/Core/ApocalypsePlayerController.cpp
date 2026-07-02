#include "Core/ApocalypsePlayerController.h"
#include "UI/HealthBarWidget.h"
#include "UI/DayCounterWidget.h"
#include "Core/ZoneManager.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CrosshairWidget.h"

void AApocalypsePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidgetClass)
	{
		HealthBarWidgetInstance = CreateWidget<UHealthBarWidget>(this, HealthBarWidgetClass);
		if (HealthBarWidgetInstance)
		{
			HealthBarWidgetInstance->AddToViewport();
		}
	}

	if (DayCounterWidgetClass)
	{
		DayCounterWidgetInstance = CreateWidget<UDayCounterWidget>(this, DayCounterWidgetClass);
		if (DayCounterWidgetInstance)
		{
			DayCounterWidgetInstance->AddToViewport();

			AZoneManager* ZoneManager = Cast<AZoneManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AZoneManager::StaticClass()));
			if (ZoneManager)
			{
				ZoneManager->OnZoneTransitionVisualsReady.AddDynamic(this, &AApocalypsePlayerController::OnDayChanged);
			}
		}
	}

	if (CrosshairWidgetClass)
	{
		CrosshairWidgetInstance = CreateWidget<UCrosshairWidget>(this, CrosshairWidgetClass);
		if (CrosshairWidgetInstance)
		{
			CrosshairWidgetInstance->AddToViewport();
		}
	}

	PrimaryActorTick.bCanEverTick = true;
}

void AApocalypsePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateCrosshairRecovery(DeltaTime);
}

void AApocalypsePlayerController::TriggerCrosshairSpread(float SpreadAmount)
{
	CurrentCrosshairSpread += SpreadAmount;
}

void AApocalypsePlayerController::UpdateCrosshairRecovery(float DeltaTime)
{
	if (!CrosshairWidgetInstance) return;
	if (FMath::IsNearlyZero(CurrentCrosshairSpread, 0.01f)) return;

	CurrentCrosshairSpread = FMath::FInterpTo(CurrentCrosshairSpread, 0.0f, DeltaTime, CrosshairRecoverySpeed);
	CrosshairWidgetInstance->SetSpread(CurrentCrosshairSpread);
}

void AApocalypsePlayerController::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if (HealthBarWidgetInstance)
	{
		HealthBarWidgetInstance->UpdateHealth(CurrentHealth, MaxHealth);
	}
}

void AApocalypsePlayerController::OnDayChanged(int32 NewDay)
{
	if (DayCounterWidgetInstance)
	{
		DayCounterWidgetInstance->UpdateDay(NewDay);
		DayCounterWidgetInstance->PlayShowAnimation();
	}
}