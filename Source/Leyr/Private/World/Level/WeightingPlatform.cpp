// @ Retropsis 2024-2025.

#include "World/Level/WeightingPlatform.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Interaction/PlayerInterface.h"

AWeightingPlatform::AWeightingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	
	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	OverlapBox->SetupAttachment(BoxCollision);
	
	Platform = EPlatformType::Weighting;
}

void AWeightingPlatform::Tick(float DeltaSeconds)
{
	AActor::Tick(DeltaSeconds);
	
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
	if(HasReachedTarget()) MovementDirection = EMovementDirection::Still;
}

void AWeightingPlatform::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AWeightingPlatform::OnBeginOverlap);
		OverlapBox->OnComponentEndOverlap.AddDynamic(this, &AWeightingPlatform::OnEndOverlap);
		
		DownPosition = RouteSpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
		UpPosition = RouteSpline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World);
	}
}

void AWeightingPlatform::MoveTo(const FVector& InCurrentTarget, float Speed, float InterpolationSpeed, float DeltaSeconds) const
{
	switch (InterpMethod) {
	case EInterpMethod::Default:
		BoxCollision->SetWorldLocation(FMath::VInterpConstantTo(BoxCollision->GetComponentLocation(), InCurrentTarget, DeltaSeconds, Speed));
		break;
	case EInterpMethod::EaseOut:
		BoxCollision->SetWorldLocation(FMath::VInterpTo(BoxCollision->GetComponentLocation(), InCurrentTarget, DeltaSeconds, InterpolationSpeed));
		break;
	case EInterpMethod::EaseIn:
		BoxCollision->SetWorldLocation(FMath::InterpEaseIn(BoxCollision->GetComponentLocation(), InCurrentTarget, DeltaSeconds, InterpolationSpeed));
		break;
	case EInterpMethod::EaseInOut:
		BoxCollision->SetWorldLocation(FMath::InterpEaseInOut(BoxCollision->GetComponentLocation(), InCurrentTarget, DeltaSeconds, InterpolationSpeed));
		break;
	}
}

void AWeightingPlatform::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetPlatformDirection(OtherActor, true);
}

void AWeightingPlatform::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SetPlatformDirection(OtherActor, false);
}

void AWeightingPlatform::SetPlatformDirection(AActor* OtherActor, bool bDownward)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		MovementDirection = bDownward ? EMovementDirection::Downward : EMovementDirection::Upward;
		CurrentTarget = bDownward ? DownPosition : UpPosition;
	}
}
