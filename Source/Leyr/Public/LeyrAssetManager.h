// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/AssetManager.h"
#include "LeyrAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API ULeyrAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static ULeyrAssetManager& Get();
 
protected:
	virtual void StartInitialLoading() override;
};
