#pragma once

#include "CoreMinimal.h"
#include "CameraData.generated.h"

UENUM(BlueprintType)
enum class EExitDirection : uint8
{
	Vertical UMETA(DisplayName="Vertical"),
	Horizontal UMETA(DisplayName="Horizontal"),
};

UENUM(BlueprintType)
enum class ECameraInterpState : uint8
{
	None,
	Entering,
	Leaving,
	Following,
	WithinBounds,
};

USTRUCT(BlueprintType)
struct FCameraBounds
{
	GENERATED_BODY()

	FCameraBounds() {}
	FCameraBounds(const FBoxSphereBounds& Bounds, const FVector& Location) :
		Left(Bounds.Origin.X - Bounds.BoxExtent.X, 10.f, Location.Z),
		Right(Bounds.Origin.X + Bounds.BoxExtent.X, 10.f, Location.Z),
		Top(Location.X, 10.f, Bounds.Origin.Z + Bounds.BoxExtent.Z),
		Bottom(Location.X, 10.f, Bounds.Origin.Z - Bounds.BoxExtent.Z),
		bInitialized(true) {}
	
	FVector Left = FVector::ZeroVector;
	FVector Right = FVector::ZeroVector;
	FVector Top = FVector::ZeroVector;
	FVector Bottom = FVector::ZeroVector;
	bool bInitialized = false;
};