// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class APlayerCharacter;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEYR_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(AActor* InteractingActor);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractHit(AActor* InteractingActor, FName BoneName);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractBottom(AActor* InteractingActor);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool ShouldBlockProjectile();
};
