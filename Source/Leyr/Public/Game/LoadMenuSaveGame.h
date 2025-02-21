// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "UI/Controller/InventoryWidgetController.h"
#include "LoadMenuSaveGame.generated.h"

struct FInventoryItemData;
class UGameplayAbility;

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken,
};

USTRUCT(BlueprintType)
struct FAbilitySaveData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	TSubclassOf<UGameplayAbility> GameplayAbility;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	FGameplayTag AbilityTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	FGameplayTag AbilityStatus = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	FGameplayTag AbilityInput = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	int32 AbilityLevel = 1;
};

inline bool operator==(const FAbilitySaveData& Left, const FAbilitySaveData& Right)
{
	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}

USTRUCT(BlueprintType)
struct FActorSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FName Name = FName();
	
	UPROPERTY()
	FTransform Transform = FTransform();

	// Serialized Actor Variables marked with SaveGame specifier
	UPROPERTY()
	TArray<uint8> Bytes;
};

inline bool operator==(const FActorSaveData& Left, const FActorSaveData& Right)
{
	return Left.Name == Right.Name;
}

USTRUCT(BlueprintType)
struct FMapSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FString AssetName = FString();

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
};

/**
 * 
 */
UCLASS()
class LEYR_API ULoadMenuSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString SlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	FString PlayerName = FString("Default Name");
	
	UPROPERTY()
	FString MapName = FString("Default Map Name");
	
	UPROPERTY()
	FString MapAssetName = FString("Default Map Asset Name");
	
	UPROPERTY()
	FName PlayerStartTag;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;

	UPROPERTY()
	bool bFirstTimeLoadIn = true;

	/*
	 * Player Data
	 */
	UPROPERTY()
	int32 Level = 1;
	
	UPROPERTY()
	int32 Experience = 0;
	
	UPROPERTY()
	int32 SkillPoints = 0;
	
	UPROPERTY()
	int32 AttributePoints = 0;

	UPROPERTY()
	float Strength = 0.f;

	UPROPERTY()
	float Dexterity = 0.f;

	UPROPERTY()
	float Vitality = 0.f;

	UPROPERTY()
	float Intelligence = 0.f;

	UPROPERTY()
	float Wisdom = 0.f;

	UPROPERTY()
	float Spirit = 0.f;

	UPROPERTY()
	TArray<FAbilitySaveData> SavedAbilities;
	
	UPROPERTY()
	TArray<FMapSaveData> SavedMaps;

	FMapSaveData GetMapSaveDataWithName(const FString& InName);
	bool HasMap(const FString& InName);

	/*
	 * Inventory
	 */
	UPROPERTY()
	TArray<FInventoryItemData> SavedItems;

	UPROPERTY()
	TMap<FGameplayTag, FEquippedItem> SavedEquippedItems;
};
