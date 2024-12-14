#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBehaviourState : uint8
{
	Patrol UMETA(DisplayName="Patrol"),
	Chase UMETA(DisplayName="Chase"),
	Fall UMETA(DisplayName="Fall"),
	Search UMETA(DisplayName="Search"),
};

UENUM(BlueprintType)
enum class EChasingState : uint8
{
	Chasing UMETA(DisplayName="Chasing"),
	HitReacting UMETA(DisplayName="HitReacting"),
	Defeat UMETA(DisplayName="Defeat"),
};

UENUM(BlueprintType)
enum class EBehaviourType : uint8
{
	Patrol UMETA(DisplayName="Patrol"),
	Ranger UMETA(DisplayName="Ranger"),
	Turret UMETA(DisplayName="Turret"),
	Airborne UMETA(DisplayName="Airborne"),
};

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	Destination UMETA(DisplayName="Destination"),
	Sine UMETA(DisplayName="Sine"),
	Spline UMETA(DisplayName="Spline"),
	Patrol UMETA(DisplayName="Patrol"),
	Points UMETA(DisplayName="Points"),
};