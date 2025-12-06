// @ Retropsis 2024-2025.


#include "Data/LevelAreaData.h"
#include "Data/EncounterSpawnData.h"

void ULevelAreaData::PreEditChange(FProperty* PropertyAboutToChange)
{
	Super::PreEditChange(PropertyAboutToChange);

	if (PropertyAboutToChange->GetName() == TEXT("EncounterSpawns"))
	{
		TempEncounterSpawns.Empty();
		TempEncounterSpawns.Append(EncounterSpawns);
	}
}

void ULevelAreaData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	/*
	 * TODO: Catch Undo, send the array  of encounter spawns to camera boundary to compare and refill
	 */

	if (PropertyChangedEvent.Property->GetName() == TEXT("EncounterSpawns"))
	{
		int32 index = PropertyChangedEvent.GetArrayIndex(TEXT("EncounterSpawns"));
		if (TempEncounterSpawns.IsValidIndex(index))
		{
			OnEncounterSpawnsPropertyDeleted.Broadcast(TempEncounterSpawns[index].EncounterSpawnTag);
			UE_LOG(LogClass, Warning, TEXT("Deleted GameplayTag %s"), *TempEncounterSpawns[index].EncounterSpawnTag.ToString());
		}
		UE_LOG(LogClass, Warning, TEXT("PropertyChain index %d"), index);
		if (EncounterSpawns.IsValidIndex(index))
		{
			UE_LOG(LogClass, Warning, TEXT("ChangeType: %u, GameplayTag %s"), PropertyChangedEvent.ChangeType, *EncounterSpawns[index].EncounterSpawnTag.ToString());
		}
		// OnEncounterSpawnsPropertyChanged.Broadcast();
	}
	
	const FName PropertyName = PropertyChangedEvent.Property->GetFName();
	// OnLevelAreaDataPropertyChanged.Broadcast(PropertyName);
	
	TempEncounterSpawns.Empty();
}

void ULevelAreaData::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	
}
