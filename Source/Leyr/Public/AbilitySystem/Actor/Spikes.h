// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Actor/DamageAbilityActor.h"
#include "Interaction/LevelActorInterface.h"
#include "World/Level/LevelDesignData.h"
#include "Spikes.generated.h"

enum class EBuildPattern : uint8;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class LEYR_API ASpikes : public ADamageAbilityActor, public ILevelActorInterface
{
	GENERATED_BODY()

public:
	ASpikes();
	virtual void OnConstruction(const FTransform& Transform) override;

	//~ LevelActorInterface
	virtual void ToggleActivate_Implementation(bool bActivate) override;
	//~ LevelActorInterface

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AbilityActor|Spikes")
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AbilityActor|Spikes")
	EBuildPattern BuildDirection = EBuildPattern::Horizontal;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AbilityActor|Spikes")
	int32 Length = 3;
};
