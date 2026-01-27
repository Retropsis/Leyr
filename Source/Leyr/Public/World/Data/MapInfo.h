// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MapInfo.generated.h"

class UPaperSprite;
/**
 * 
 */
UENUM(BlueprintType)
enum class EBiome : uint8
{
	Default,
};

USTRUCT(BlueprintType)
struct FParallaxLayerData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly) TObjectPtr<UPaperSprite> Sprite = nullptr;
	UPROPERTY(EditDefaultsOnly) bool bIsVisible = false;
	UPROPERTY(EditDefaultsOnly) FVector2D ScrollingRatio = FVector2D::ZeroVector;
	UPROPERTY(EditDefaultsOnly) float ScrollingSpeed = 0.f;
	UPROPERTY(EditDefaultsOnly) float OffsetY = 0.f;
};

USTRUCT(BlueprintType)
struct FMapDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Map Defaults") EBiome Biome = EBiome::Default;
	
	UPROPERTY(EditDefaultsOnly, Category="Map Defaults") FParallaxLayerData SkyData = FParallaxLayerData();
	UPROPERTY(EditDefaultsOnly, Category="Map Defaults") FParallaxLayerData SunData = FParallaxLayerData();
	UPROPERTY(EditDefaultsOnly, Category="Map Defaults") FParallaxLayerData FirstMoonData = FParallaxLayerData();
	UPROPERTY(EditDefaultsOnly, Category="Map Defaults") FParallaxLayerData SecondMoonData = FParallaxLayerData();
	UPROPERTY(EditDefaultsOnly, Category="Map Defaults") FParallaxLayerData BackCloudsData = FParallaxLayerData();
	UPROPERTY(EditDefaultsOnly, Category="Map Defaults") FParallaxLayerData FrontCloudsData = FParallaxLayerData();
	UPROPERTY(EditDefaultsOnly, Category="Map Defaults") FParallaxLayerData BackLayerData = FParallaxLayerData();
	UPROPERTY(EditDefaultsOnly, Category="Map Defaults") FParallaxLayerData FrontLayerData = FParallaxLayerData();
	UPROPERTY(EditDefaultsOnly, Category="Map Defaults") FParallaxLayerData ForegroundFogLayerData = FParallaxLayerData();
	UPROPERTY(EditDefaultsOnly, Category="Map Defaults") FParallaxLayerData BackgroundFogLayerData = FParallaxLayerData();
	UPROPERTY(EditDefaultsOnly, Category="Map Defaults") FParallaxLayerData ForegroundLayerData = FParallaxLayerData();
};

USTRUCT()
struct FExplorationData
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly) uint32 SubdivisionExploredCount = 0;
	UPROPERTY(VisibleDefaultsOnly) uint32 SubdivisionTotalCount = 0;
};

UCLASS()
class LEYR_API UMapInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FMapDefaultInfo> MapInformation;
	
	FMapDefaultInfo GetMapDefaultInfo(FName MapName);
};
