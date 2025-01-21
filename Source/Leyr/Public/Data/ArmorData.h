// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemData.h"
#include "ArmorData.generated.h"

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	Head UMETA(DisplayName="Head"),
	UpperBody UMETA(DisplayName="UpperBody"),
	LowerBody UMETA(DisplayName="LowerBody"),
	Feet UMETA(DisplayName="Feet"),
	Wrist UMETA(DisplayName="Wrist"),
	Waist UMETA(DisplayName="Waist"),
	Neck UMETA(DisplayName="Neck")
};

/**
 * 
 */
UCLASS()
class LEYR_API UArmorData : public UItemData
{
	GENERATED_BODY()

public:
	UArmorData();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Armor")
	EArmorType ArmorType = EArmorType::Head;
};
