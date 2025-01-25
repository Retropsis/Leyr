// @ Retropsis 2024-2025.

#include "World/Level/Helper/ActorBuilder.h"
#include "Components/BillboardComponent.h"

AActorBuilder::AActorBuilder()
{
	PrimaryActorTick.bCanEverTick = false;

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>("BillboardComponent");
	BillboardComponent->SetupAttachment(GetRootComponent());
}

