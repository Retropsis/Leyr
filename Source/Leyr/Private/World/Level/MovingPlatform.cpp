// @ Retropsis 2024-2025.

#include "World/Level/MovingPlatform.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"

AMovingPlatform::AMovingPlatform()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	BoxCollision->SetupAttachment(Root);
	
	RouteSpline = CreateDefaultSubobject<USplineComponent>("Route");
	RouteSpline->SetupAttachment(Root);
	Platform = EPlatformType::Moving;
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	CurrentTarget = RouteSpline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
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
