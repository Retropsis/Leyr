// @ Retropsis 2024-2025.


#include "UI/Widget/BaseCommonActivatableWidget.h"

void UBaseCommonActivatableWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
