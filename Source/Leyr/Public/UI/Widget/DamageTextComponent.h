// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "UI/Data/UIData.h"
#include "DamageTextComponent.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(const FUIMessageData& MessageData);
};
