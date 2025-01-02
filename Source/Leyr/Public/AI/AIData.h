#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EEncounterSize : uint8
{
	Default UMETA(DisplayName="Default"),
	Critter UMETA(DisplayName="Critter"),
	Humanoid UMETA(DisplayName="Humanoid"),
	Large UMETA(DisplayName="Large"),
	Boss UMETA(DisplayName="Boss"),
};

UENUM(BlueprintType)
enum class EEncounterName : uint8
{
	Default UMETA(DisplayName="Default"),
	Skeleton UMETA(DisplayName="Skeleton"),
	Goblin UMETA(DisplayName="Goblin"),
	Necromancer UMETA(DisplayName="Necromancer"),
	Archer UMETA(DisplayName="Archer [Patrol]"),
	FlyingEye UMETA(DisplayName="FlyingEye"),
	Bat UMETA(DisplayName="Bat"),
	FlameThrower UMETA(DisplayName="FlameThrower [Turret]"),
	ArrowTrap UMETA(DisplayName="ArrowTrap [Turret]"),
	Merman UMETA(DisplayName="Merman"),
	Crocodile UMETA(DisplayName="Crocodile"),
	Shark UMETA(DisplayName="Shark"),
	Pterodactyl UMETA(DisplayName="Pterodactyl"),

	// Bosses
	TheScourge UMETA(DisplayName="TheScourge"),
};

UENUM(BlueprintType)
enum class EBehaviourState : uint8
{
	Patrol UMETA(DisplayName="Patrol"),
	Chase UMETA(DisplayName="Chase"),
	Dive UMETA(DisplayName="Dive"),
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
	Aquatic UMETA(DisplayName="Aquatic"),
};

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	Immobile UMETA(DisplayName="Immobile"),
	NavMesh UMETA(DisplayName="NavMesh"),
	Destination UMETA(DisplayName="Destination"),
	Sine UMETA(DisplayName="Sine"),
	Spline UMETA(DisplayName="Spline"),
	Patrol UMETA(DisplayName="Patrol"),
	Points UMETA(DisplayName="Points"),
};