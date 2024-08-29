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
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	// TSoftClassPtr<AItem> ItemClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Weapon")
	float BaseDamage = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Weapon")
	bool bUseAmmunition = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Weapon")
	int32 Ammunition = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item|Weapon")
	int32 MaxAmmunition = 0;
};
