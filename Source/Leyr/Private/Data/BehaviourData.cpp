// @ Retropsis 2024-2025.


#include "Data/BehaviourData.h"

void UBehaviourData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	const FName PropertyName = PropertyChangedEvent.Property->GetFName();
	OnBehaviourDataPropertyChanged.Broadcast(PropertyName);
	 if (PropertyName == FName("MovementType"))
	 {
	 	OnMovementTypePropertyChanged.Broadcast(MovementType);
	 }
}
