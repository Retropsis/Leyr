// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AICharacterAnimInstance.h"
#include "MultiPartAnimInstance.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class EDirection : uint8
{
	Left,
	Right,
	Downward,
	Upward,
};

class AAICharacter;
/**
 * 
 */
UCLASS()
class LEYR_API UMultiPartAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	TObjectPtr<AAICharacter> AICharacter;
	
	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	bool bIsMoving;
	
	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	bool bAirborne;

	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadWrite, Category="AICharacter")
	FVector MovementDirection = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	EDefeatState DefeatState = EDefeatState::None;
	
	UPROPERTY(BlueprintReadWrite, Category="AICharacter")
	EDirection Direction = EDirection::Downward;
	
	UPROPERTY(BlueprintReadWrite, Category="AICharacter")
	EDirection PreviousDirection = EDirection::Downward;

	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	bool bRequestTurning = false;
	
	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	bool bIsDiving = false;
	
	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	bool bIsCombatTargetDefeated = false;
};
