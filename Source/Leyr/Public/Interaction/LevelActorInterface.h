// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LevelActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULevelActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEYR_API ILevelActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void ToggleActivate(bool bActivate);
};
