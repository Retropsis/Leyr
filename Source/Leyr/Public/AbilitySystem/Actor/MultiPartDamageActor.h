// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Actor/DamageAbilityActor.h"
#include "Interaction/InteractionInterface.h"
#include "MultiPartDamageActor.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API AMultiPartDamageActor : public ADamageAbilityActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AMultiPartDamageActor();
	virtual void InteractHit_Implementation(AActor* InteractingActor, FName BoneName) override;

	UFUNCTION(BlueprintImplementableEvent)
	void HandleDetachment(FName BoneName);
	
	UFUNCTION(BlueprintImplementableEvent)
	void HandleBeginOverlap();
	
protected:
	UFUNCTION(BlueprintCallable)
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditDefaultsOnly)
	float ImpactImpulse = 100.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBreak = false;
	
	UPROPERTY(EditDefaultsOnly)
	FName UnbreakableRoot = FName();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> MultiPartFlipbook;
};
