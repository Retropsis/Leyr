// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemData.h"
#include "WeaponData.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UWeaponData : public UItemData
{
	GENERATED_BODY()

public:
	UWeaponData();
	virtual FString GetDescription(UAttributeInfo* AttributeInfo, int32 Level) override;
};
