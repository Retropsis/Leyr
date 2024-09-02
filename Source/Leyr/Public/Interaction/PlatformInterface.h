// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlatformInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlatformInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEYR_API IPlatformInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void SetBoxCollisionEnabled(bool bEnabled);
};
