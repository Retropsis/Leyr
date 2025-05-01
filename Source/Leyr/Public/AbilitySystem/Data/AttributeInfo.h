// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FBaseAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Attributes"))
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY( BlueprintReadOnly)
	float AttributeValue = 0.f;

	UPROPERTY( BlueprintReadOnly)
	float AttributeBaseValue = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayAttribute AttributeGetter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> AttributeIcon = nullptr;
};

/**
 * 
 */
UCLASS()
class LEYR_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FBaseAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;
	FBaseAttributeInfo FindAttributeInfoForName(const FString& Name, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="AttributeName"))
	TArray<FBaseAttributeInfo> AttributeInformation;
};
