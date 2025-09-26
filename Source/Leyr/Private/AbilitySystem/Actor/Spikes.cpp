// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/Spikes.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"

ASpikes::ASpikes()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	SetRootComponent(BoxCollision);
}

void ASpikes::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	BoxCollision->SetBoxExtent(FVector(BuildDirection == EBuildPattern::Horizontal ? Length * 32.f : 16.f, 100.f, BuildDirection == EBuildPattern::Vertical ? Length * 32.f : 16.f));
}

void ASpikes::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ASpikes::OnBeginOverlap);
	}
	SetActorTickEnabled(false);
}

void ASpikes::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->Implements<UPlayerInterface>()) CauseDamage(OtherActor);
	// if(OtherActor->ActorHasTag("Player")) CauseDamage(OtherActor);
}

void ASpikes::ToggleActivate_Implementation(bool bActivate)
{
	SetActorTickEnabled(bActivate);
}
