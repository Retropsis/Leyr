// @ Retropsis 2024-2025.

#include "Game/LeyrGameMode.h"

#include "EngineUtils.h"
#include "LeyrLogChannels.h"
#include "Data/ItemDataRow.h"
#include "Game/LeyrGameInstance.h"
#include "Game/LoadMenuSaveGame.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
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
	LoadMenuSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	LoadMenuSaveGame->MapAssetName = LoadSlot->MapAssetName;
	LoadMenuSaveGame->SaveSlotStatus = Taken;

	UGameplayStatics::SaveGameToSlot(LoadMenuSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

void ALeyrGameMode::TravelToMap(UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->SlotIndex;
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

AActor* ALeyrGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	ULeyrGameInstance* LeyrGameInstance = Cast<ULeyrGameInstance>(GetGameInstance());
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if(Actors.Num() > 0)
	{
		AActor* SelectedActor = Actors[0];
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if(PlayerStart->PlayerStartTag == LeyrGameInstance->PlayerStartTag)
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

ULoadMenuSaveGame* ALeyrGameMode::RetrieveInGameSaveData() const
{
	const ULeyrGameInstance* LeyrGameInstance = Cast<ULeyrGameInstance>(GetGameInstance());
	const FString InGameLoadSlotName = LeyrGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = LeyrGameInstance->LoadSlotIndex;

	return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
}

void ALeyrGameMode::SaveInGameProgressData(ULoadMenuSaveGame* SaveObject) const
{
	ULeyrGameInstance* LeyrGameInstance = Cast<ULeyrGameInstance>(GetGameInstance());
	const FString InGameLoadSlotName = LeyrGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = LeyrGameInstance->LoadSlotIndex;
	LeyrGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;
	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}

void ALeyrGameMode::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);
	
	const ULeyrGameInstance* LeyrGameInstance = Cast<ULeyrGameInstance>(GetGameInstance());
	check(LeyrGameInstance);

	if (ULoadMenuSaveGame* SaveGame = GetSaveSlotData(LeyrGameInstance->LoadSlotName, LeyrGameInstance->LoadSlotIndex))
	{
		if (DestinationMapAssetName != FString(""))
		{
			SaveGame->MapAssetName = DestinationMapAssetName;
			SaveGame->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
		}
		if (!SaveGame->HasMap(WorldName))
		{
			FMapSaveData NewMapData;
			NewMapData.AssetName = WorldName;
			SaveGame->SavedMaps.Add(NewMapData);
		}
		FMapSaveData SavedMap = SaveGame->GetMapSaveDataWithName(WorldName);
		SavedMap.SavedActors.Empty();

		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			if(!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;
			
			FActorSaveData ActorData;
			ActorData.Name = Actor->GetFName();
			ActorData.Transform = Actor->GetTransform();

			FMemoryWriter MemoryWriter(ActorData.Bytes);
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;
			Actor->Serialize(Archive);
			SavedMap.SavedActors.AddUnique(ActorData);
		}

		for (FMapSaveData& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.AssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}
		UGameplayStatics::SaveGameToSlot(SaveGame, LeyrGameInstance->LoadSlotName, LeyrGameInstance->LoadSlotIndex);
	}
}

void ALeyrGameMode::LoadWorldState(UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);
	
	const ULeyrGameInstance* LeyrGameInstance = Cast<ULeyrGameInstance>(GetGameInstance());
	check(LeyrGameInstance);

	if (UGameplayStatics::DoesSaveGameExist(LeyrGameInstance->LoadSlotName, LeyrGameInstance->LoadSlotIndex))
	{
		ULoadMenuSaveGame* SaveGame = Cast<ULoadMenuSaveGame>(UGameplayStatics::LoadGameFromSlot(LeyrGameInstance->LoadSlotName, LeyrGameInstance->LoadSlotIndex));
		if (SaveGame == nullptr)
		{
			UE_LOG(LogLeyr, Error, TEXT("Failed to load slot"));
			return;;
		}
		
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<USaveInterface>()) continue;

			for (FActorSaveData ActorData : SaveGame->GetMapSaveDataWithName(WorldName).SavedActors)
			{
				if(ActorData.Name == Actor->GetFName())
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(ActorData.Transform);
					}
					FMemoryReader MemoryReader(ActorData.Bytes);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive);
					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
}

FString ALeyrGameMode::GetMapNameFromMapAssetName(const FString& MapAssetName) const
{
	for (const TTuple<FString, TSoftObjectPtr<UWorld>>& Map : Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == MapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
}

void ALeyrGameMode::PlayerDefeated(ACharacter* Character)
{
	ULoadMenuSaveGame* SaveGame = RetrieveInGameSaveData();
	if (!IsValid(SaveGame)) return;

	UGameplayStatics::OpenLevel(Character, FName(SaveGame->MapName));
}