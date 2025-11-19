#pragma once

#include "CoreMinimal.h"
#include "AIData.generated.h"

class UPaperZDAnimInstance;
class ASplineComponentActor;

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
enum class EBehaviourState : uint8
{
	Patrol UMETA(DisplayName="Patrol"),
	Chase UMETA(DisplayName="Chase"),
	Dive UMETA(DisplayName="Dive"),
	Timeline UMETA(DisplayName="Timeline"),
	Fall UMETA(DisplayName="Fall"),
	Search UMETA(DisplayName="Search"),
	Duck UMETA(DisplayName="Duck"),
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

UENUM(BlueprintType)
enum class ETimelineMovementType : uint8
{	
	Vertical,
	Horizontal,
	Mixed,
};

USTRUCT(BlueprintType)
struct FPatternParams
{
	GENERATED_BODY();

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed = -1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<ASplineComponentActor*> SplineComponentActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UPaperZDAnimInstance> PaperAnimInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> MultiPartAnimInstance = nullptr;
};