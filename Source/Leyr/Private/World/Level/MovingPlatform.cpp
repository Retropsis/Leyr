// @ Retropsis 2024-2025.

#include "World/Level/MovingPlatform.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "World/Level/Event/Lever.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	RouteSpline = CreateDefaultSubobject<USplineComponent>("Route");
	RouteSpline->SetupAttachment(Root);
	
	BoxCollision->SetupAttachment(Root);
	
	Platform = EPlatformType::Moving;
}

#if WITH_EDITOR
void AMovingPlatform::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if(!IsValid(RouteSpline)) return;
	for (int i = 0; i < RouteSpline->GetNumberOfSplinePoints(); ++i)
	{
		RouteSpline->SetSplinePointType(i, ESplinePointType::Linear);
	}
}
#endif

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(bIsActivated) Move(DeltaSeconds);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	CurrentTarget = RouteSpline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
	bIsActivated = !IsValid(Switch);
	if(IsValid(Switch)) Switch->OnEventStateChanged.AddLambda([this] (const EEventState NewState)
	{
		bIsActivated = NewState == EEventState::On;
	});
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
	if(HasReachedTarget()) ChooseNextStep();
}

void AMovingPlatform::ChooseNextStep()
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
