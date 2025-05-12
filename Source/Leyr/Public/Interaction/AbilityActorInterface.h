// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/ActorClassInfo.h"
#include "UObject/Interface.h"
#include "AbilityActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAbilityActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEYR_API IAbilityActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DestroyActor();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDestroyed() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetActorLevel();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EActorClass GetActorClass();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool ShouldShowFloatingText();
	
	// FOnASCRegistered& GetOnASCRegistered();
	// FOnDeath& GetOnDeath();
};
