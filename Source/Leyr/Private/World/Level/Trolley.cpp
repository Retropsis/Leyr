// @ Retropsis 2024-2025.

#include "World/Level/Trolley.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"

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

void ATrolley::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->ActorHasTag("Player"))
	{
		GetWorld()->GetTimerManager().SetTimer(OverlapWaitTimer, this, &ATrolley::HandleOverlapWaitTimeEnd, OverlapWaitTime);
	}
}

void ATrolley::ChooseNextStep()
{
	// CurrentTarget = RouteSpline->GetLocationAtSplinePoint(2, ESplineCoordinateSpace::World);
	// MovingSpeed = 2000.f;
	SetLifeSpan(3.f);
	BoxCollision->SetSimulatePhysics(true);
	BoxCollision->SetEnableGravity(true);
}
