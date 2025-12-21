// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Actor/Breakable.h"
#include "Interaction/LevelActorInterface.h"
#include "BreakableContainer.generated.h"

UCLASS()
class LEYR_API ABreakableContainer : public ABreakable, public ILevelActorInterface
{
	GENERATED_BODY()

public:
	ABreakableContainer();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnLoot();
	
	//~ LevelActorInterface
	virtual void ResetState_Implementation() override;
	//~ LevelActorInterface

protected:
	virtual void DestroyActor_Implementation() override;
};
