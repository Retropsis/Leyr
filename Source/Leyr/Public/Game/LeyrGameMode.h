// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Inventory/InventoryComponent.h"
#include "LeyrGameMode.generated.h"

class ULootDataSet;
class ULoadMenuSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UInventoryCostData;
class UMapInfo;
class UBehaviourInfo;
class UEncounterInfo;
class UActorClassInfo;
class UAbilityInfo;
class UItemAbilityInfo;
class UCharacterClassInfo;

/**
 * 
 */
UCLASS()
class LEYR_API ALeyrGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Actor Class Defaults")
	TObjectPtr<UActorClassInfo> ActorClassInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TObjectPtr<UEncounterInfo> EncounterInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Loot Info")
	TObjectPtr<ULootDataSet> LootDataSet;
	
	/* Maps */
	UPROPERTY(EditDefaultsOnly, Category = "Map Info")
	TObjectPtr<UMapInfo> MapInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Map Info")
	FString DefaultMapName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Map Info")
	FName DefaultPlayerStartTag;

	UPROPERTY(EditDefaultsOnly, Category = "Map Info")
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly, Category = "Map Info")
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	void TravelToMap(UMVVM_LoadSlot* Slot);
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	/* Inventory */
	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<UInventoryCostData> InventoryCostInfo;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<UDataTable> ItemDataTable;

	FInventoryItemData FindItemDataByID(int32 ItemID) const;
	FInventoryItemData FindItemDataByRowName(FName RowName) const;

	/* Saving */
	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);

	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex) const;
	ULoadMenuSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;
	ULoadMenuSaveGame* RetrieveInGameSaveData() const;
	void SaveInGameProgressData(ULoadMenuSaveGame* SaveObject) const;

	void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString("")) const;
	void LoadWorldState(UWorld* World) const;
	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadMenuSaveGameClass;

	UFUNCTION(BlueprintCallable)
	void PlayerDefeated(ACharacter* Character);
	
	UFUNCTION(BlueprintCallable)
	void BackToMainMenu(ACharacter* Character);

protected:
	virtual void BeginPlay() override;
};
