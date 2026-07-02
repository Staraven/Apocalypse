// Copyright (c) 2024 Synty Studios Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class POLYGONAPOCALYPSE_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public: 
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealth(float CurrentHealth, float MaxHealth);
	
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;
};
