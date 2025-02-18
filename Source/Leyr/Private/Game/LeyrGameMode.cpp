// @ Retropsis 2024-2025.

#include "Game/LeyrGameMode.h"
#include "Data/ItemDataRow.h"
#include "Game/LoadMenuSaveGame.h"
#include "Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadFile.h"

FInventoryItemData ALeyrGameMode::FindItemDataByID(int32 ItemID) const
{
	checkf(ItemDataTable, TEXT("Items Data Talbe is missing please fill in [%s]"), *GetName())

	const FString ID = FString::Printf(TEXT("%d"), ItemID);
	if (FItemDataRow* ItemDataRow = ItemDataTable->FindRow<FItemDataRow>(*ID, *ID))
	{
		return ItemDataRow->ItemData;
	}
	return FInventoryItemData();
}

FInventoryItemData ALeyrGameMode::FindItemDataByRowName(FName RowName) const
{
	checkf(ItemDataTable, TEXT("Items Data Talbe is missing please fill in [%s]"), *GetName())

	if (FItemDataRow* ItemDataRow = ItemDataTable->FindRow<FItemDataRow>(RowName, RowName.ToString()))
	{
		return ItemDataRow->ItemData;
	}
	return FInventoryItemData();
}

void ALeyrGameMode::SaveSlotData(UMVVM_LoadFile* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->LoadSlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->LoadSlotName, SlotIndex);
	}
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadMenuSaveGameClass);
	ULoadMenuSaveGame* LoadMenuSaveGame = Cast<ULoadMenuSaveGame>(SaveGameObject);
	LoadMenuSaveGame->PlayerName = LoadSlot->PlayerName;

	UGameplayStatics::SaveGameToSlot(LoadMenuSaveGame, LoadSlot->LoadSlotName, SlotIndex);
}
