#include "UI/DayCounterWidget.h"

void UDayCounterWidget::UpdateDay(int32 CurrentDay)
{
	if (DayText)
	{
		FString DisplayText = FString::Printf(TEXT("Day %d"), CurrentDay);
		DayText->SetText(FText::FromString(DisplayText));
	}
	

}