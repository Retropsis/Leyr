// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ConstructionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UConstructionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEYR_API IConstructionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowTopSprite(bool bShow);
};
