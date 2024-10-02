// @ Retropsis 2024-2025.

#include "Inventory/Container/Container.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Inventory/ContainerComponent.h"
#include "Leyr/Leyr.h"

AContainer::AContainer()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	OverlapBox->SetupAttachment(GetRootComponent());
	OverlapBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapBox->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);

	Container = CreateDefaultSubobject<UContainerComponent>("ContainerComponent");
}

void AContainer::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority()) Container->SetSlotCount(SlotCount);
}

void AContainer::Interact_Implementation(AActor* InteractingActor)
{
	IPlayerInterface::Execute_SetContainer(InteractingActor, this);
	InteractingActors.AddUnique(InteractingActor);

	for (int i = 0; i < Container->Items.Num(); ++i)
	{
		if (Container->Items[i].ID == 0)
		{
			IPlayerInterface::Execute_ResetInventorySlot(InteractingActor, EContainerType::Container, i);
		}
		else
		{
			IPlayerInterface::Execute_UpdateInventorySlot(InteractingActor, EContainerType::Container, i, Container->Items[i]);
		}
	}
}

void AContainer::StopInteracting_Implementation(AActor* InteractingActor)
{
	InteractingActors.Remove(InteractingActor);
}

void AContainer::ForEachActorUpdateInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData Item)
{
	for (AActor* Actor : InteractingActors)
	{
		if(Actor) IPlayerInterface::Execute_UpdateInventorySlot(Actor, ContainerType, SlotIndex, Item);
	}
}

void AContainer::ForEachActorResetInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex)
{
	for (AActor* Actor : InteractingActors)
	{
		if(Actor) IPlayerInterface::Execute_ResetInventorySlot(Actor, ContainerType, SlotIndex);
	}
}

void AContainer::UpdateContainer_Implementation()
{
	for (AActor* Actor : InteractingActors)
	{
		if(Actor) IPlayerInterface::Execute_UpdateContainerSlots(Actor, Container->Items.Num());
		for (int i = 0; i < Container->Items.Num(); ++i)
		{
			if (Container->Items[i].ID == 0)
			{
				IPlayerInterface::Execute_ResetInventorySlot(Actor, EContainerType::Container, i);
			}
			else
			{
				IPlayerInterface::Execute_UpdateInventorySlot(Actor, EContainerType::Container, i, Container->Items[i]);
			}
		}
	}
}

