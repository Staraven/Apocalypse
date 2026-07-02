#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ApocalypsePlayerController.generated.h"

UCLASS()
class POLYGONAPOCALYPSE_API AApocalypsePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UHealthBarWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UDayCounterWidget> DayCounterWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UCrosshairWidget> CrosshairWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void TriggerCrosshairSpread(float SpreadAmount);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void UpdateCrosshairRecovery(float DeltaTime);

private:
	UPROPERTY()
	class UHealthBarWidget* HealthBarWidgetInstance;

	UPROPERTY()
	class UDayCounterWidget* DayCounterWidgetInstance;

	UFUNCTION()
	void OnDayChanged(int32 NewDay);

	UPROPERTY()
	class UCrosshairWidget* CrosshairWidgetInstance;

	float CurrentCrosshairSpread = 0.0f;

	UPROPERTY(EditAnywhere, Category = "UI")
	float CrosshairRecoverySpeed = 8.0f;
};