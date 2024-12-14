// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MapInfo.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EBiome : uint8
{
	Default,
};

USTRUCT(BlueprintType)
struct FMapDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	EBiome Biome = EBiome::Default;
};

UCLASS()
class LEYR_API UMapInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Encounter Defaults")
	TMap<FName, FMapDefaultInfo> MapInformation;
	
	FMapDefaultInfo GetMapDefaultInfo(FName MapName);
};
