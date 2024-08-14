// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "UI/Controller/WidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FGameplayTag;
class UAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FBaseAttributeInfo&, Info);

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
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag) const;
};
