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
	USoundBase* ImpactSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector BoxTraceStart = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector BoxTraceEnd = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector BoxTraceExtent = FVector(40.f, 20.f, 30.f);
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
	HitReact UMETA(DisplayName="HitReact"),
	Crouching UMETA(DisplayName="Crouching"),
	UnCrouching UMETA(DisplayName="UnCrouching"),
	Attacking UMETA(DisplayName="Attacking"),
	Falling UMETA(DisplayName="Falling"),
	HangingLedge UMETA(DisplayName="Hanging Ledge"),
	HangingRope UMETA(DisplayName="Hanging Rope"),
	ClimbingRope UMETA(DisplayName="Climbing Rope"),
	HangingLadder UMETA(DisplayName="Hanging Ladder"),
	OnGroundSlope UMETA(DisplayName="OnGroundSlope"),
	OnRopeSlope UMETA(DisplayName="OnRopeSlope"),
	Entangled UMETA(DisplayName="Entangled"),
	Swimming UMETA(DisplayName="Swimming"),
	
	Dodging UMETA(DisplayName="Dodging"),
	Rolling UMETA(DisplayName="Rolling"),
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
