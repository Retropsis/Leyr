// @ Retropsis 2024-2025.

#include "World/Level/MovingPlatform.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	BoxCollision->SetupAttachment(Root);
	
	RouteSpline = CreateDefaultSubobject<USplineComponent>("Route");
	RouteSpline->SetupAttachment(Root);
	Platform = EPlatformType::Moving;
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Move(DeltaSeconds);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	CurrentTarget = RouteSpline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
}

void AMovingPlatform::Move(float DeltaSeconds)
{
	switch (InterpMethod) {
	case EInterpMethod::Default:
		BoxCollision->SetWorldLocation(FMath::VInterpConstantTo(BoxCollision->GetComponentLocation(), CurrentTarget, DeltaSeconds, MovingSpeed));
		break;
	case EInterpMethod::EaseOut:
		BoxCollision->SetWorldLocation(FMath::VInterpTo(BoxCollision->GetComponentLocation(), CurrentTarget, DeltaSeconds, InterpSpeed));
		break;
	case EInterpMethod::EaseIn:
		BoxCollision->SetWorldLocation(FMath::InterpEaseIn(BoxCollision->GetComponentLocation(), CurrentTarget, DeltaSeconds, InterpSpeed));
		break;
	case EInterpMethod::EaseInOut:
		BoxCollision->SetWorldLocation(FMath::InterpEaseInOut(BoxCollision->GetComponentLocation(), CurrentTarget, DeltaSeconds, InterpSpeed));
		break;
	}
	if(HasReachedTarget()) ChooseNextTarget();
}

void AMovingPlatform::ChooseNextTarget()
{
	CurrentIndex++;
	if(CurrentIndex >= RouteSpline->GetNumberOfSplinePoints())
	{
		CurrentIndex = 0;
	}
	CurrentTarget = RouteSpline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
}

bool AMovingPlatform::HasReachedTarget() const
{
	return FVector::Distance(CurrentTarget, BoxCollision->GetComponentLocation()) < TargetTolerance;
}
