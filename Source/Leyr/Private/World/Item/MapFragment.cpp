// @ Retropsis 2024-2025.

#include "World/Item/MapFragment.h"
#include "UI/Component/MapComponent.h"

AMapFragment::AMapFragment()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMapFragment::Interact_Implementation(AActor* InteractingActor)
{
	Super::Interact_Implementation(InteractingActor);
	if (const APlayerController* PlayerController = Cast<APlayerController>(InteractingActor->GetInstigatorController()))
	{
		if (UMapComponent* MapComponent = PlayerController->FindComponentByClass<UMapComponent>())
		{
			MapComponent->UnveilRooms(RoomsToUnveil);
		}
	}
}

