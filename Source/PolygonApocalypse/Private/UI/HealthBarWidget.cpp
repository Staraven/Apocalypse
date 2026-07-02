// Copyright (c) 2024 Synty Studios Limited. All rights reserved.


#include "UI/HealthBarWidget.h"

void UHealthBarWidget::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthProgressBar && MaxHealth > 0.0f)
	{
		float Percent = CurrentHealth / MaxHealth;
		HealthProgressBar->SetPercent(Percent);
	}
}