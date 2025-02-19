// @ Retropsis 2024-2025.

#include "Game/LeyrGameMode.h"
#include "Data/ItemDataRow.h"
#include "Game/LoadMenuSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void ALeyrGameMode::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName, DefaultMap);
}

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

void ALeyrGameMode::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

void ALeyrGameMode::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	DeleteSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadMenuSaveGameClass);
	ULoadMenuSaveGame* LoadMenuSaveGame = Cast<ULoadMenuSaveGame>(SaveGameObject);
	LoadMenuSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadMenuSaveGame->MapName = LoadSlot->GetMapName();
	LoadMenuSaveGame->SaveSlotStatus = Taken;

	UGameplayStatics::SaveGameToSlot(LoadMenuSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

void ALeyrGameMode::TravelToMap(UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->SlotIndex;
	
	;UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

AActor* ALeyrGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if(Actors.Num() > 0)
	{
		AActor* SelectedActor = Actors[0];
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if(PlayerStart->PlayerStartTag == FName("TrueStart"))
				{
					SelectedActor = PlayerStart;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}

ULoadMenuSaveGame* ALeyrGameMode::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadMenuSaveGameClass);
	}
	ULoadMenuSaveGame* LoadMenuSaveGame = Cast<ULoadMenuSaveGame>(SaveGameObject);
	return LoadMenuSaveGame;
}
