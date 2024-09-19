// @ Retropsis 2024-2025.

#include "World/Level/Trolley.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Leyr/Leyr.h"

ATrolley::ATrolley()
{
	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	OverlapBox->SetupAttachment(GetRootComponent());
	bIsActivated = false;
	CurrentIndex = 1;
}

void ATrolley::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(BoxCollision->IsGravityEnabled()) BoxCollision->SetRelativeRotation(FRotator::ZeroRotator);
}

void ATrolley::BeginPlay()
{
	AActor::BeginPlay();
	CurrentTarget = RouteSpline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
	if (HasAuthority()) OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ATrolley::OnBeginOverlap);
}

void ATrolley::HandleOverlapWaitTimeEnd() { bIsActivated = true; }


void ATrolley::HandleFallingTimeEnd()
{
	if(const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(RespawnTimer, this, &ATrolley::HandleRespawnTimeEnd, RespawnTime);
	if(FallingTimer.IsValid()) FallingTimer.Invalidate();
	OnTrolleyFalling.Broadcast();
}

void ATrolley::HandleRespawnTimeEnd()
{
	BoxCollision->SetSimulatePhysics(false);
	bIsActivated = false;
	CurrentIndex = 1;
	CurrentTarget = RouteSpline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
	BoxCollision->SetWorldLocation(RouteSpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World));
	OverlapBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapBox->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	OverlapBox->SetCollisionResponseToChannel(ECC_Enemy, ECR_Overlap);
	if(RespawnTimer.IsValid()) RespawnTimer.Invalidate();
	OnTrolleyRespawn.Broadcast();
}

void ATrolley::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->ActorHasTag("Player"))
	{
		if(const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(OverlapWaitTimer, this, &ATrolley::HandleOverlapWaitTimeEnd, OverlapWaitTime);
	}
}

void ATrolley::ChooseNextStep()
{
	if(bIsSingleUse) SetLifeSpan(3.f);
	else if(const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(FallingTimer, this, &ATrolley::HandleFallingTimeEnd, FallingTime);
	}
	BoxCollision->SetSimulatePhysics(true);
	if(OverlapWaitTimer.IsValid()) OverlapWaitTimer.Invalidate();
}
