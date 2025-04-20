// @ Retropsis 2024-2025.

#include "UI/Widget/BaseCommonUserWidget.h"

void UBaseCommonUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
