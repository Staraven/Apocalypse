// Copyright (c) 2024 Synty Studios Limited. All rights reserved.

#include "TimeCycle/DayNightCycle.h"
#include "Engine/DirectionalLight.h"
#include "Components/LightComponent.h"
#include "EngineUtils.h"
#include "Components/SkyLightComponent.h"

ADayNightCycle::ADayNightCycle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADayNightCycle::BeginPlay()
{
	Super::BeginPlay();
}

void ADayNightCycle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTime += DeltaTime / DayDuration;

	if (CurrentTime >= 1.0f) CurrentTime = 0.0f;

	UpdateSunRotation();
}

void ADayNightCycle::UpdateSunRotation()
{
	if (!SunLight) return;

	float SunAngle = CurrentTime * 360.0f;
	SunLight->SetActorRotation(FRotator(SunAngle - 90.0f, 0.0f, 0.0f));

	if (SkySphere)
	{
		UFunction* RefreshFunc = SkySphere->FindFunction(TEXT("RefreshMaterial"));
		if (RefreshFunc)
		{
			SkySphere->ProcessEvent(RefreshFunc, nullptr);
		}
	}

	if (SceneSkyLight)
	{
		float Intensity = FMath::Clamp(FMath::Sin(CurrentTime * PI), 0.0f, 1.0f);
		SceneSkyLight->GetLightComponent()->SetIntensity(Intensity * 2.0f);
	}

	if (SunLight)
	{
		float SunHeight = FMath::Sin(CurrentTime * PI * 2.0f + PI * 0.5f);

		float SunIntensity = FMath::Max(SunHeight * 20.0f, 0.01f);
		SunLight->GetLightComponent()->SetIntensity(SunIntensity);

		FLinearColor DayColor = FLinearColor(1.0f, 0.9f, 0.7f);
		FLinearColor NightColor = FLinearColor(0.1f, 0.1f, 0.3f);
		float BlendAlpha = FMath::Clamp((SunHeight + 1.0f) / 2.0f, 0.0f, 1.0f);
		FLinearColor CurrentColor = FLinearColor::LerpUsingHSV(NightColor, DayColor, BlendAlpha);
		SunLight->GetLightComponent()->SetLightColor(CurrentColor);
	}

	if (MoonLight)
	{
		float SunHeight = FMath::Sin(CurrentTime * PI * 2.0f + PI * 0.5f);
		float NightBlend = FMath::Clamp(-SunHeight, 0.0f, 1.0f);

		MoonLight->GetLightComponent()->SetIntensity(NightBlend * 0.5f);
		MoonLight->GetLightComponent()->SetVisibility(NightBlend > 0.01f);
	}

	if (SceneSkyLight)
	{
		float SunHeight = FMath::Sin(CurrentTime * PI * 2.0f + PI * 0.5f);
		float BlendAlpha = FMath::Clamp((SunHeight + 1.0f) / 2.0f, 0.0f, 1.0f);

		float SkyIntensity = FMath::Lerp(0.05f, 1.0f, BlendAlpha);
		SceneSkyLight->GetLightComponent()->SetIntensity(SkyIntensity);

		FLinearColor DayColor = FLinearColor(1.0f, 0.95f, 0.8f);
		FLinearColor NightColor = FLinearColor(0.1f, 0.15f, 0.4f);
		FLinearColor SkyColor = FLinearColor::LerpUsingHSV(NightColor, DayColor, BlendAlpha);
		SceneSkyLight->GetLightComponent()->SetLightColor(SkyColor);
	}
}

bool ADayNightCycle::IsNight() const
{
	return CurrentTime < 0.25f || CurrentTime > 0.75f;
}