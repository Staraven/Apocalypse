#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZoneManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZoneTimerUpdated, float, TimeRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZoneChanged, int32, NewZoneIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRestPointAvailable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZoneTransitionVisualsReady, int32, DayNumber);

UCLASS()
class POLYGONAPOCALYPSE_API AZoneManager : public AActor
{
	GENERATED_BODY()

public:
	AZoneManager();

	// WaveManager instances used in zone order.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	TArray<class AWaveManager*> ZoneWaveManagers;

	// Duration of the day phase for each zone.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	float DayDuration = 120.0f;

	// Index of the active zone.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zone")
	int32 CurrentZoneIndex = 0;

	UPROPERTY(BlueprintAssignable, Category = "Zone")
	FOnZoneTimerUpdated OnZoneTimerUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Zone")
	FOnZoneChanged OnZoneChanged;

	UPROPERTY(BlueprintAssignable, Category = "Zone")
	FOnRestPointAvailable OnRestPointAvailable;

	UFUNCTION(BlueprintCallable, Category = "Zone")
	void StartCurrentZone();

	UFUNCTION(BlueprintCallable, Category = "Zone")
	void OnPlayerRested();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone|Fade")
	float FadeOutDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone|Fade")
	float FadeInDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone|Fade")
	FLinearColor FadeColor = FLinearColor::Black;

	UPROPERTY(BlueprintAssignable, Category = "Zone")
	FOnZoneTransitionVisualsReady OnZoneTransitionVisualsReady;

	UFUNCTION()
	void OnZoneWaveCompleted(int32 CompletedDay);

protected:
	virtual void BeginPlay() override;

private:
	void UpdateZoneTimer();

	FTimerHandle ZoneTimerHandle;
	float TimeRemaining = 0.0f;
	bool bWaitingForRest = false;

	// Performs the actual zone switch after the fade-out finishes.
	void DoZoneTransition();
	FTimerHandle FadeTransitionTimerHandle;

	void OnFadeInCompleted();
	FTimerHandle FadeInCompletedTimerHandle;

	// Starts the fade-in and notifies the UI when it finishes.
	void PlayFadeInAndNotify();
};