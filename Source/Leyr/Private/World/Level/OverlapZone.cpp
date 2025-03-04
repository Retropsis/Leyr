// @ Retropsis 2024-2025.

#include "World/Level/OverlapZone.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Leyr/Leyr.h"

AOverlapZone::AOverlapZone()
{
	PrimaryActorTick.bCanEverTick = false;
	OverlapZone = CreateDefaultSubobject<UBoxComponent>("OverlapZone");
	OverlapZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapZone->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	SetRootComponent(OverlapZone);
	
#if WITH_EDITORONLY_DATA
	LevelDesignIcon = CreateDefaultSubobject<UBillboardComponent>("LevelDesignIcon");
	LevelDesignIcon->SetupAttachment(OverlapZone);
#endif
}

void AOverlapZone::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		OverlapZone->OnComponentBeginOverlap.AddDynamic(this, &AOverlapZone::OnBeginOverlap);
		OverlapZone->OnComponentEndOverlap.AddDynamic(this, &AOverlapZone::OnEndOverlap);
	}
}

void AOverlapZone::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_HandleOverlapZone(OtherActor, RequiredState, false);
	}
}

void AOverlapZone::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_HandleOverlapZone(OtherActor, RequiredState, true);
	}
}

