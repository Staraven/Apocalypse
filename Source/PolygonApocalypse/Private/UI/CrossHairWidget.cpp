#include "UI/CrosshairWidget.h"

void UCrosshairWidget::SetSpread(float SpreadAmount)
{
	if (LineUp)    LineUp->SetRenderTranslation(FVector2D(0.0f, -SpreadAmount));
	if (LineDown)  LineDown->SetRenderTranslation(FVector2D(0.0f, SpreadAmount));
	if (LineLeft)  LineLeft->SetRenderTranslation(FVector2D(-SpreadAmount, 0.0f));
	if (LineRight) LineRight->SetRenderTranslation(FVector2D(SpreadAmount, 0.0f));
}