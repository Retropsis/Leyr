// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LeyrGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API ULeyrGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName PlayerStartTag = FName("EditorStart");

	UPROPERTY()
	FString LoadSlotName = FString();
	
	UPROPERTY()
	int32 LoadSlotIndex = 0;
};
