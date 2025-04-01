#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Leyr/Leyr.h"
#include "CombatData.generated.h"

class UPaperZDAnimSequence;

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FValueRange AbilityPower = FValueRange();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Damage"))
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Montage"))
	FGameplayTag MontageTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="CombatSocket"))
	FGameplayTag SocketTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPaperZDAnimSequence* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USoundBase> ImpactSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector BoxTraceStart = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector BoxTraceEnd = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector BoxTraceExtent = FVector(40.f, 20.f, 30.f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0.f, ClampMax=1.f))
	float HitStopAmount = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HitStopDuration = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Poise = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector HitImpulse = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FBoxTraceData
{
	GENERATED_BODY()
	
	UPROPERTY() FVector Start = FVector::ZeroVector;
	UPROPERTY() FVector End = FVector::ZeroVector;
	UPROPERTY() FVector Extent = FVector(40.f, 20.f, 30.f);
};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	Unoccupied UMETA(DisplayName="Unoccupied"),
	Crouching UMETA(DisplayName="Crouching"),
	Attacking UMETA(DisplayName="Attacking"),
	Falling UMETA(DisplayName="Falling"),
	HangingLedge UMETA(DisplayName="Hanging Ledge"),
	HangingRope UMETA(DisplayName="Hanging Rope"),
	HangingHook UMETA(DisplayName="Hanging Hook"),
	HangingLadder UMETA(DisplayName="Hanging Ladder"),
	HoppingLedge UMETA(DisplayName="Hopping Ledge"),
	Climbing UMETA(DisplayName="Climbing"),
	OnElevator UMETA(DisplayName="OnElevator"),
	OnGroundSlope UMETA(DisplayName="OnGroundSlope"),
	OnRopeSlope UMETA(DisplayName="OnRopeSlope"),
	Entangled UMETA(DisplayName="Entangled"),
	Swimming UMETA(DisplayName="Swimming"),
	Walking UMETA(DisplayName="Walking"),
	WalkingPeaceful UMETA(DisplayName="WalkingPeaceful"),
	Aiming UMETA(DisplayName="Aiming"),
	Casting UMETA(DisplayName="Casting"),

	/* Locked Animations */
	Dodging UMETA(DisplayName="Dodging"),
	Rolling UMETA(DisplayName="Rolling"),
	HitReact UMETA(DisplayName="HitReact"),
	Stunned UMETA(DisplayName="Stunned"),
	ClimbingRope UMETA(DisplayName="Climbing Rope"),
	UnCrouching UMETA(DisplayName="UnCrouching"),
	RollingEnd UMETA(DisplayName="RollingEnd"),
	Defeated UMETA(DisplayName="Defeated"),	
};

UENUM(BlueprintType)
enum class ECombatDirection : uint8
{
	None UMETA(DisplayName="None"),
	Upward UMETA(DisplayName="Upward"),
	Forward UMETA(DisplayName="Forward"),
	Downward UMETA(DisplayName="Downward"),
	ForwardUp UMETA(DisplayName="ForwardUp"),
	ForwardDown UMETA(DisplayName="ForwardDown"),
};

UENUM(BlueprintType)
enum class ECombatStance : uint8
{
	Unequipped UMETA(DisplayName="Unequipped"),
	OneHanded UMETA(DisplayName="OneHanded"),
	TwoHanded UMETA(DisplayName="TwoHanded"),
	DualWield UMETA(DisplayName="DualWield"),
	Rogue UMETA(DisplayName="Rogue"),
	Buckler UMETA(DisplayName="Buckler"),
	Rod UMETA(DisplayName="Rod"),
};

UENUM(BlueprintType)
enum class EDefeatState : uint8
{
	None UMETA(DisplayName="None"),
	Defeated UMETA(DisplayName="Defeated"),
	Executed UMETA(DisplayName="Executed"),
	KnockedOut UMETA(DisplayName="KnockedOut"),
};