// @ Retropsis 2024-2025.


#include "Data/EncounterData.h"

void UEncounterData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	const FName PropertyName = PropertyChangedEvent.Property->GetFName();
	OnEncounterDataPropertyChanged.Broadcast(PropertyName);
}
