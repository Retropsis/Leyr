 // @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "Data/CombatData.h"
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

	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	EDefeatState DefeatState = EDefeatState::None;
	
	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	bool bIsDiving = false;
	
	UPROPERTY(BlueprintReadOnly, Category="AICharacter")
	bool bIsCombatTargetDefeated = false;
};
