// @ Retropsis 2024-2025.

#include "UI/ViewModel/MVVM_LoadMenu.h"

#include "Game/LeyrGameInstance.h"
#include "Game/LeyrGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadMenu::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);
	
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);
	
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);
}

void UMVVM_LoadMenu::LoadData()
{
	ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(LeyrGameMode)) return;;
	
	for (TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		const ULoadMenuSaveGame* SaveObject = LeyrGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);
		LoadSlot.Value->SetPlayerName(SaveObject->PlayerName);
		LoadSlot.Value->SetMapName(SaveObject->MapName);
		LoadSlot.Value->SetPlayerLevel(SaveObject->Level);
		LoadSlot.Value->SlotStatus =  SaveObject->SaveSlotStatus;
		LoadSlot.Value->PlayerStartTag =  SaveObject->PlayerStartTag;
		LoadSlot.Value->InitializeSlot();
	}
}

UMVVM_LoadSlot* UMVVM_LoadMenu::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadMenu::NewSlotButtonPressed(int32 SlotIndex, const FString& EnteredName)
{
	ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(LeyrGameMode))
	{
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Magenta, FString("Switch to Single Player"));
		return;
	}
	
	LoadSlots[SlotIndex]->SetMapName(LeyrGameMode->DefaultMapName);
	LoadSlots[SlotIndex]->SetPlayerName(EnteredName);
	LoadSlots[SlotIndex]->SetPlayerLevel(1);
	LoadSlots[SlotIndex]->SlotStatus = Taken;
	LoadSlots[SlotIndex]->PlayerStartTag = LeyrGameMode->DefaultPlayerStartTag;
	LoadSlots[SlotIndex]->MapAssetName = LeyrGameMode->DefaultMap.ToSoftObjectPath().GetAssetName();
	
	LeyrGameMode->SaveSlotData(LoadSlots[SlotIndex], SlotIndex);
	LoadSlots[SlotIndex]->InitializeSlot();

	ULeyrGameInstance* LeyrGameInstance = Cast<ULeyrGameInstance>(LeyrGameMode->GetGameInstance());
	LeyrGameInstance->LoadSlotName = LoadSlots[SlotIndex]->GetLoadSlotName();
	LeyrGameInstance->LoadSlotIndex = LoadSlots[SlotIndex]->SlotIndex;
	LeyrGameInstance->PlayerStartTag = LeyrGameMode->DefaultPlayerStartTag;
}

void UMVVM_LoadMenu::NewGameButtonPressed(int32 SlotIndex)
{
	LoadSlots[SlotIndex]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadMenu::SelectSlotButtonPressed(int32 SlotIndex)
{
	SlotSelected.Broadcast();
	for (TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		LoadSlot.Value->EnableSelectSlotButton.Broadcast(LoadSlot.Key != SlotIndex);
	}
	SelectedSlot = LoadSlots[SlotIndex];
}

void UMVVM_LoadMenu::DeleteButtonPressed()
{
	if(IsValid(SelectedSlot))
	{
		ALeyrGameMode::DeleteSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->SlotIndex);
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->EnableSelectSlotButton.Broadcast(true);
	}
}

void UMVVM_LoadMenu::PlayButtonPressed()
{
	ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(this));
	ULeyrGameInstance* LeyrGameInstance = Cast<ULeyrGameInstance>(LeyrGameMode->GetGameInstance());
	LeyrGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
	LeyrGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
	LeyrGameInstance->LoadSlotIndex = SelectedSlot->SlotIndex;
	
	if(IsValid(SelectedSlot)) LeyrGameMode->TravelToMap(SelectedSlot);
}
