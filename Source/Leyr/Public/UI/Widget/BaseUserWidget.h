// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interaction/BaseUserWidgetInterface.h"
#include "BaseUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UBaseUserWidget : public UUserWidget, public IBaseUserWidgetInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
