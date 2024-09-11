// @ Retropsis 2024-2025.

#include "World/Level/PendulumPlatform.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

APendulumPlatform::APendulumPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Center = CreateDefaultSubobject<USceneComponent>("Center");
	SetRootComponent(Center);
	BoxCollision->SetupAttachment(GetRootComponent());
	
	FirstPosition = CreateDefaultSubobject<USceneComponent>("FirstPosition");
	FirstPosition->SetupAttachment(GetRootComponent());
	
	ApexPosition = CreateDefaultSubobject<USceneComponent>("ApexPosition");
	ApexPosition->SetupAttachment(GetRootComponent());
}

void APendulumPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	BoxCollision->SetRelativeLocation(FVector(UKismetMathLibrary::DegCos(SpeedX * Angle) * RadiusX, 0.f, UKismetMathLibrary::DegSin(SpeedY * Angle) * RadiusY));
	
	float Delta = FVector::DotProduct(GetActorForwardVector().GetSafeNormal(), (BoxCollision->GetComponentLocation() - GetActorLocation()).GetSafeNormal());

	switch (ClockDirection) {
	case EClockDirection::Clockwise:
		Angle = FMath::Modulo(Angle - 50.f * DeltaSeconds, 360.f);
		if(FMath::IsNearlyEqual(Delta, MinAngle, 0.01f)) ClockDirection = EClockDirection::CounterClockwise;
		break;
	case EClockDirection::CounterClockwise:
		Angle = FMath::Modulo(Angle + 50.f * DeltaSeconds, 360.f);
		if(FMath::IsNearlyEqual(Delta, MaxAngle, 0.01f)) ClockDirection = EClockDirection::Clockwise;
		break;
	}
}

void APendulumPlatform::BeginPlay()
{
	Super::BeginPlay();
	RadiusX = FMath::Abs(FirstPosition->GetRelativeLocation().X);
	RadiusY = FMath::Abs(ApexPosition->GetRelativeLocation().Z);

	MinAngle = FVector::DotProduct(GetActorForwardVector().GetSafeNormal(), (FirstPosition->GetComponentLocation() - GetActorLocation()).GetSafeNormal());
	MaxAngle = MinAngle * -1.f;
}
