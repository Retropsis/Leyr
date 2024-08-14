// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputConfig.generated.h"

USTRUCT(BlueprintType)
struct FBaseInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(Categories="InputTag"))
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class LEYR_API UInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FBaseInputAction> AbilityInputActions;
};
