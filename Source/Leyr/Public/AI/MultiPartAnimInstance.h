// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Data/CombatData.h"
#include "MultiPartAnimInstance.generated.h"

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
	
	UPROPERTY(BlueprintReadWrite, Category="AICharacter")
	ERelativeDirection RelativeDirection = ERelativeDirection::Forward;
	
	UPROPERTY(BlueprintReadWrite, Category="AICharacter")
	ERelativeDirection PreviousRelativeDirection = ERelativeDirection::Forward;

	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	bool bRequestTurning = false;
	
	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	bool bIsDiving = false;
	
	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	bool bIsCombatTargetDefeated = false;
};
