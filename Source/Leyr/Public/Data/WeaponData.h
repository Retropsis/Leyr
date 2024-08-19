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
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tool")
	// TSoftClassPtr<AItemBase> ToolClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float BaseDamage = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	bool bUseAmmunition = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	int32 Ammunition = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	int32 MaxAmmunition = 0;
};
