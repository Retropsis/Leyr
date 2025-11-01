// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Cluster.generated.h"

class UBoxComponent;

UCLASS()
class LEYR_API ACluster : public AActor
{
	GENERATED_BODY()

public:
	ACluster();
	void InitializeCluster(const FAdditionalEffectParams& Params, UStatusEffectData* EffectData, UNiagaraSystem* Effect, USoundBase* Sound);

protected:
	virtual void BeginPlay() override;

private:
	void CauseDamage(AActor* OtherActor);

	UFUNCTION()
	virtual void OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void EffectStart() const;
	void EffectEnded() const;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> HitCollision;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> VisualEffect;
	
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 3.f;

	UPROPERTY(EditDefaultsOnly)
	float EffectiveTime = .5f;
	
	UPROPERTY(EditDefaultsOnly)
	float EffectDelay = .5f;
		
	TWeakObjectPtr<UStatusEffectData> StatusEffectData = nullptr;
	
	UPROPERTY() FAdditionalEffectParams DamageEffectParams;
	UPROPERTY() TObjectPtr<UNiagaraSystem> ImpactEffect;
	UPROPERTY() TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditDefaultsOnly)
	bool bDebug = false;
};
