// @ Retropsis 2024-2025.

#include "World/Item.h"
#include "PaperFlipbookComponent.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Data/ItemDataRow.h"
#include "Interaction/InventoryInterface.h"
#include "Kismet/GameplayStatics.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetRenderComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if(ChangedPropertyName == GET_MEMBER_NAME_CHECKED(FDataTableRowHandle, RowName))
	{
		if(ItemDataTable)
		{
			if(const FItemDataRow* ItemDataRow = ItemDataTable->FindRow<FItemDataRow>(ItemRowHandle.RowName, ItemRowHandle.RowName.ToString()))
			{
				ItemData = ItemDataRow->ItemData;
				PickupSound = ItemDataRow->PickupSound;
				GetRenderComponent()->SetFlipbook(ItemDataRow->PickupFlipbook);
				SetActorLabel(FString::Printf(TEXT("BP_%s"), *ItemData.Name.ToString()));
			}
		}
	}
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		if(!ItemRowHandle.RowName.IsNone())
		{
			ItemData = ULeyrAbilitySystemLibrary::FindItemDataByRowName(this, ItemRowHandle.RowName);
		}
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnBeginOverlap);
		Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
		if(ItemData.ID == 0)
		{
			
		}
	}
}

void AItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UInventoryComponent* InventoryComponent = IInventoryInterface::Execute_GetInventoryComponent(OtherActor))
	{
		// if (InventoryComponent->TryAddItem(ItemData)) Destroy();
	}
}

void AItem::Interact_Implementation(AActor* InteractingActor)
{	
	if (UInventoryComponent* InventoryComponent = IInventoryInterface::Execute_GetInventoryComponent(InteractingActor))
	{
		InventoryComponent->ServerAddItem(ItemData);
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		Destroy();
	}
}
