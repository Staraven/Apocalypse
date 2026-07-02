#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "CrosshairWidget.generated.h"

UCLASS()
class POLYGONAPOCALYPSE_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Crosshair")
	void SetSpread(float SpreadAmount);

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* LineUp;

	UPROPERTY(meta = (BindWidget))
	UImage* LineDown;

	UPROPERTY(meta = (BindWidget))
	UImage* LineLeft;

	UPROPERTY(meta = (BindWidget))
	UImage* LineRight;
};