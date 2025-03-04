// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/PhysicsConstrainedActor.h"
#include "PCA_Brazier.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class LEYR_API APCA_Brazier : public APhysicsConstrainedActor
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	float ImpactImpulse = 100.f;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> ImpactSound;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool bHit = false;
};
