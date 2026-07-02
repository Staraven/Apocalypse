#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "DayCounterWidget.generated.h"

UCLASS()
class POLYGONAPOCALYPSE_API UDayCounterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void UpdateDay(int32 CurrentDay);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Wave")
	void PlayShowAnimation();

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DayText;
};