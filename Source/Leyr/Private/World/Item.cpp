// @ Retropsis 2024-2025.

#include "World/Item.h"
#include "Components/SphereComponent.h"
#include "Interaction/InventoryInterface.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
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
		if (InventoryComponent->TryAddItem(ItemData)) Destroy();
	}
}
