// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "LeyrCheatManagerExtension.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API ULeyrCheatManagerExtension : public UCheatManagerExtension
{
	GENERATED_BODY()

public:
	ULeyrCheatManagerExtension();
	
	UFUNCTION(Exec)
	void Invincibility(bool bEnable) const;
	
	UFUNCTION(Exec)
	void Teleport(FName Tag) const;
};
