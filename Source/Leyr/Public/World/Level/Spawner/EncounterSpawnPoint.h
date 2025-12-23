// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "EncounterSpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API AEncounterSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	AEncounterSpawnPoint();
	void SetEncounterIcon(UTexture2D* Icon) const;
	void SetLabel(const FString& Label);
};
