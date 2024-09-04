// @ Retropsis 2024-2025.

#include "World/Level/Platform.h"
#include "Components/BoxComponent.h"

APlatform::APlatform()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	SetRootComponent(BoxCollision);

	Tags.Add(FName("Platform"));
}