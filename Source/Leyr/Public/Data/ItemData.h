// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

struct FGameplayModifierInfo;
class UGameplayEffect;
class AItem;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Misc UMETA(DisplayName="Misc"),
	Resource UMETA(DisplayName="Resource"),
	Equipment UMETA(DisplayName="Equipment"),
	Armor UMETA(DisplayName="Armor"),
	Consumable UMETA(DisplayName="Consumable"),
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class LEYR_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	int32 ID = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FText Name = FText();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FText Description = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	bool bIsStackable = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	int32 StackSize = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	EItemType ItemType = EItemType::Misc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	TArray<TSubclassOf<UGameplayEffect>> Effects;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	TArray<FGameplayModifierInfo> Modifiers;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	float Weight = 0.f;
};
