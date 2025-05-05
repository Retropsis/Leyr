// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Actor/AbilityActor.h"
#include "MultiPartAbilityActor.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API AMultiPartAbilityActor : public AAbilityActor
{
	GENERATED_BODY()

public:
	AMultiPartAbilityActor();
	
protected:
	UFUNCTION(BlueprintCallable)
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditDefaultsOnly)
	float ImpactImpulse = 100.f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> MultiPartFlipbook;
};