// @ Retropsis 2024-2025.

#include "World/Level/Helper/ActorBuilder.h"

#include "Interaction/ConstructionInterface.h"

AActorBuilder::AActorBuilder()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AActorBuilder::Build()
{
	if (ChildActorClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Please fill up ChildActorClass in [%s]"), *GetName());
		return;
	}
	
	ClearChildActors();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	for (int i = 0; i < WidthCount; ++i)
	{
		for (int j = 0; j < HeightCount; ++j)
		{
			FVector Location{ GetActorLocation().X + i * TileWidth, 0.f, GetActorLocation().Z + j * TileHeight };
			AActor* Actor = GetWorld()->SpawnActor<AActor>(ChildActorClass, Location, FRotator::ZeroRotator, SpawnParams);
			if (Actor && Actor->Implements<UConstructionInterface>()) IConstructionInterface::Execute_ShowTopSprite(Actor, bShowTopSprite);
			ChildActors.Add(Actor);
		}
	}
}

void AActorBuilder::ClearChildActors()
{
	for (AActor* Actor : ChildActors)
	{
		if(Actor) Actor->Destroy();
	}
	ChildActors.Empty();
}