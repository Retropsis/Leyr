 // @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "Data/CombatData.h"
#include "Interaction/MultiPartAIInterface.h"
#include "AICharacterAnimInstance.generated.h"

enum class EDefeatState : uint8;
class AAICharacter;
/**
 * 
 */
UCLASS()
class LEYR_API UAICharacterAnimInstance : public UPaperZDAnimInstance
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnInit_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void SetMultiPartAnimInstance(const EMultiPartAnimationState State) { MultiPartAnimationState = State; }
	
	UFUNCTION(BlueprintCallable)
	void SetIsFlipped(const bool Flipped) { bIsFlipped = Flipped; }
	
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
	ERelativeDirection RelativeDirection = ERelativeDirection::Forward;
	
	UPROPERTY(BlueprintReadWrite, Category="AICharacter")
	ERelativeDirection PreviousRelativeDirection = ERelativeDirection::Forward;

	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	EDefeatState DefeatState = EDefeatState::None;

	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	EMultiPartAnimationState MultiPartAnimationState = EMultiPartAnimationState::Idle;
	
	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	bool bIsDiving = false;
	
	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	bool bIsCombatTargetDefeated = false;
	
	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	bool bIsFlipped = false;
};
