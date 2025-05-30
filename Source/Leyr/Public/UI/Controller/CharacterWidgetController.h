// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "UI/Controller/WidgetController.h"
#include "CharacterWidgetController.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class LEYR_API UCharacterWidgetController : public UWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

private:
	UFUNCTION() void OnXPChanged(int32 NewXP);
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag) const;
};
