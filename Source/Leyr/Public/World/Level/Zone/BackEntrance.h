// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/OverlapZone.h"
#include "BackEntrance.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API ABackEntrance : public AOverlapZone
{
	GENERATED_BODY()

public:
	ABackEntrance();
	virtual bool TryBackEntrance_Implementation(AActor* InteractingActor, float CapsuleHalfHeight) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<ABackEntrance> PairedBackEntrance;
};
