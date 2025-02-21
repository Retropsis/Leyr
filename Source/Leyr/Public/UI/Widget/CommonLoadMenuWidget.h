// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonLoadMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UCommonLoadMenuWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintInitializeWidget();
};
