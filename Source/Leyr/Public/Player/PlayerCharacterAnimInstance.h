// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class LEYR_API UPlayerCharacterAnimInstance : public UPaperZDAnimInstance
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnInit_Implementation() override;
	
	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<APlayerCharacter> PlayerCharacter;
	
	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bIsMoving;
	
	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bAirborne;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bIsAccelerating;
};
