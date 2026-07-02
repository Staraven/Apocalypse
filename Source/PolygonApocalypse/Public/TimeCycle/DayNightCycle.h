// Copyright (c) 2024 Synty Studios Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DirectionalLight.h"
#include "GameFramework/Actor.h"
#include "Engine/SkyLight.h"
#include "DayNightCycle.generated.h"

UCLASS()
class POLYGONAPOCALYPSE_API ADayNightCycle : public AActor
{
	GENERATED_BODY()

public:
	ADayNightCycle();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float DayDuration = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float CurrentTime = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	ADirectionalLight* SunLight;

	UFUNCTION(BlueprintCallable, Category = "Time")
	bool IsNight() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	AActor* SkySphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	ASkyLight* SceneSkyLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	ADirectionalLight* MoonLight;

protected:
	virtual void BeginPlay() override;

private:
	void UpdateSunRotation();
};