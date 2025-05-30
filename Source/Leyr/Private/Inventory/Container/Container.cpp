// @ Retropsis 2024-2025.

#include "Inventory/Container/Container.h"
#include "PaperFlipbookComponent.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Data/ContainerData.h"
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

void AContainer::LoadActor_Implementation()
{
	if (bInitialized)
	{
		Container->Items = Items;
		Container->SetSlotCount( FMath::Max(Container->Items.Num(), 1));
	}
}

#if WITH_EDITOR
void AContainer::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if(ChangedPropertyName == GET_MEMBER_NAME_CHECKED(AContainer, ContainerData))
	{
		if(ContainerData)
		{
			if (const UContainerArtData* Data = ContainerData->ContainerArt.LoadSynchronous())
			{
				OpenLidSound = Data->OpenLidSound;
                CloseLidSound = Data->CloseLidSound;
                GetRenderComponent()->SetFlipbook(Data->ContainerFlipbook);
				GetRenderComponent()->SetPlayRate(0.f);
			}
			SetActorLabel(FString::Printf(TEXT("BP_%s"), *ContainerData->Name.ToString()));
		}
	}
}
#endif

void AContainer::BeginPlay()
{
	Super::BeginPlay();
	BuildContainerLoot();
}

void AContainer::BuildContainerLoot()
{
	if(!HasAuthority() || bInitialized) return;
	checkf(ContainerData, TEXT("Please add a DataAsset to this container: %s"), *GetName());
	
	Container->SetSlotCount(ContainerData->Items.Num());
	
	for (FContainerItem ContainerItem : ContainerData->Items)
	{
		if(ContainerItem.Item.RowName.IsNone()) continue;
		
		FInventoryItemData ItemToAdd = ULeyrAbilitySystemLibrary::FindItemDataByRowName(this, ContainerItem.Item.RowName);
		ItemToAdd.Quantity = ContainerItem.Quantity;
		ItemToAdd.EquipmentSlot = ItemToAdd.Asset.LoadSynchronous()->EquipmentSlot;
		Container->ServerAddItem(ItemToAdd);
	}
	bInitialized = true;
}

void AContainer::Interact_Implementation(AActor* InteractingActor)
{
	if(!IsContainerAccessed()) ToggleContainerLid(true);
	
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

void AContainer::ServerStopInteracting_Implementation(AActor* InteractingActor)
{
	InteractingActors.Remove(InteractingActor);
	if(!IsContainerAccessed()) ToggleContainerLid(false);
}

void AContainer::ServerForEachActorUpdateInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData Item)
{
	for (AActor* Actor : InteractingActors)
	{
		if(Actor) IPlayerInterface::Execute_UpdateInventorySlot(Actor, ContainerType, SlotIndex, Item);
	}
}

void AContainer::ServerForEachActorResetInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex)
{
	for (AActor* Actor : InteractingActors)
	{
		if(Actor) IPlayerInterface::Execute_ResetInventorySlot(Actor, ContainerType, SlotIndex);
	}
}

void AContainer::ServerUpdateContainer_Implementation()
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

