// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WorldUtility.generated.h"

enum class ESubdivisionSide : uint8;

UCLASS()
class LEYR_API UWorldUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static float GetAngleBetweenPoints(const FVector& Point_A, const FVector& Point_B);
	static ESubdivisionSide GetSubdivisionSideFromAngle(const float Angle);
	static FIntPoint GetWorldCoordinates(const FVector& WorldLocation);
};
