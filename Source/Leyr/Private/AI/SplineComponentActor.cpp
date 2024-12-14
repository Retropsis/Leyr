// @ Retropsis 2024-2025.

#include "AI/SplineComponentActor.h"
#include "Components/SplineComponent.h"

ASplineComponentActor::ASplineComponentActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	SetRootComponent(SplineComponent);
}

