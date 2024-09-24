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

	ContainerComponent = CreateDefaultSubobject<UContainerComponent>("ContainerComponent");
}

void AContainer::Interact_Implementation(AActor* InteractingActor)
{
	IPlayerInterface::Execute_SetContainer(InteractingActor, this);
}

void AContainer::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority()) ContainerComponent->SetSlotCount(SlotCount);
}

