// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/Spikes.h"
#include "Components/BoxComponent.h"

ASpikes::ASpikes()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	SetRootComponent(BoxCollision);
}

void ASpikes::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ASpikes::OnBeginOverlap);
	}
}

void ASpikes::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Player")) CauseDamage(OtherActor);
}
