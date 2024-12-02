// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/Spikes.h"
#include "Components/BoxComponent.h"

ASpikes::ASpikes()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	SetRootComponent(BoxCollision);
}

void ASpikes::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	BoxCollision->SetBoxExtent(FVector(BuildDirection == EBuildDirection::Horizontal ? Length * 32.f : 16.f, 100.f, BuildDirection == EBuildDirection::Vertical ? Length * 32.f : 16.f));
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
