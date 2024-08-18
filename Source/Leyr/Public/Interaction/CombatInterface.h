// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UPaperZDAnimSequence;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEYR_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32 GetCharacterLevel() { return 0; }
	virtual FVector GetCombatSocketLocation() { return FVector(); }
	virtual void Die() = 0;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UPaperZDAnimSequence* GetHitReactSequence();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetAttackAnimationData(FVector& InBoxTraceStart, FVector& InBoxTraceEnd);
};
