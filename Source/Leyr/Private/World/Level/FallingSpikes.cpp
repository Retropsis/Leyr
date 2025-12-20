// @ Retropsis 2024-2025.

#include "World/Level/FallingSpikes.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AFallingSpikes::AFallingSpikes()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Anchor = CreateDefaultSubobject<USceneComponent>("Anchor");
	Anchor->SetupAttachment(Root);
	
	RouteSpline = CreateDefaultSubobject<USplineComponent>("Route");
	RouteSpline->SetupAttachment(Root);
}

void AFallingSpikes::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	for (int i = 0; i < RouteSpline->GetNumberOfSplinePoints(); ++i)
	{
		RouteSpline->SetSplinePointType(i, ESplinePointType::Linear);
	}
}

void AFallingSpikes::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	switch (MovementDirection)
	{
	case EMovementDirection::Still:
		break;
	case EMovementDirection::Downward:
		MoveTo(DownPosition, DownwardSpeed, DownwardInterpSpeed, DeltaSeconds);
		break;
	case EMovementDirection::Upward:
		MoveTo(UpPosition, UpwardSpeed, UpwardInterpSpeed, DeltaSeconds);
		break;
	}
	if(HasReachedTarget())
	{
		if(MovementDirection == EMovementDirection::Upward)
		{
			MovementDirection = EMovementDirection::Still;
			GetWorld()->GetTimerManager().SetTimer(UpPositionTimer, this, &AFallingSpikes::HandleUpPositionTimeEnd, UpPositionTime);
		}
		if(MovementDirection == EMovementDirection::Downward)
		{
			MovementDirection = EMovementDirection::Still;
			GetWorld()->GetTimerManager().SetTimer(DownPositionTimer, this, &AFallingSpikes::HandleDownPositionTimeEnd, DownPositionTime);
		}
	}
	UKismetSystemLibrary::DrawDebugPoint(this, CurrentTarget, 15.f, FLinearColor::White);
}

void AFallingSpikes::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{		
		UpPosition = RouteSpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
		DownPosition = RouteSpline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World);
		GetWorld()->GetTimerManager().SetTimer(UpPositionTimer, this, &AFallingSpikes::HandleUpPositionTimeEnd, UpPositionTime);
	}
	SetActorTickEnabled(false);
}

void AFallingSpikes::ToggleActivate_Implementation(bool bActivate)
{
	GEngine->AddOnScreenDebugMessage(-1, 90.f, FColor::Green, bActivate ? "true" : "false");
	SetActorTickEnabled(bActivate);
}

void AFallingSpikes::MoveTo(const FVector& InCurrentTarget, float Speed, float InterpolationSpeed, float DeltaSeconds) const
{
	switch (InterpMethod) {
	case EInterpMethod::Default:
		Anchor->SetWorldLocation(FMath::VInterpConstantTo(Anchor->GetComponentLocation(), InCurrentTarget, DeltaSeconds, Speed));
		break;
	case EInterpMethod::EaseOut:
		Anchor->SetWorldLocation(FMath::VInterpTo(Anchor->GetComponentLocation(), InCurrentTarget, DeltaSeconds, InterpolationSpeed));
		break;
	case EInterpMethod::EaseIn:
		Anchor->SetWorldLocation(FMath::InterpEaseIn(Anchor->GetComponentLocation(), InCurrentTarget, DeltaSeconds, InterpolationSpeed));
		break;
	case EInterpMethod::EaseInOut:
		Anchor->SetWorldLocation(FMath::InterpEaseInOut(Anchor->GetComponentLocation(), InCurrentTarget, DeltaSeconds, InterpolationSpeed));
		break;
	}
}

bool AFallingSpikes::HasReachedTarget() const
{
	return FVector::Distance(CurrentTarget, Anchor->GetComponentLocation()) < TargetTolerance;
}

void AFallingSpikes::HandleUpPositionTimeEnd()
{
	CurrentTarget = DownPosition;
	MovementDirection = EMovementDirection::Downward;
}

void AFallingSpikes::HandleDownPositionTimeEnd()
{
	CurrentTarget = UpPosition;
	MovementDirection = EMovementDirection::Upward;
}
