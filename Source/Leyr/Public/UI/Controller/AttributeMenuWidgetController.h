// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "OverlayWidgetController.h"
#include "UI/Controller/WidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FGameplayTag;
class UAttributeInfo;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class LEYR_API UAttributeMenuWidgetController : public UWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
	
	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag) const;
};
