// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Actor/AbilityActor.h"
#include "Interaction/InteractionInterface.h"
#include "MultiPartAbilityActor.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API AMultiPartAbilityActor : public AAbilityActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AMultiPartAbilityActor();
	virtual void InteractHit_Implementation(AActor* InteractingActor) override;

	UFUNCTION(BlueprintImplementableEvent)
	void HandleDetachment();
	
protected:
	UFUNCTION(BlueprintCallable)
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditDefaultsOnly)
	float ImpactImpulse = 100.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBreak = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> MultiPartFlipbook;
};