// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilityTypes.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;

UCLASS()
class LEYR_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	void SetImpactEffect(UNiagaraSystem* Effect) { ImpactEffect = Effect; }
	void SetImpactSound(USoundBase* Sound) { ImpactSound = Sound; }
	void SetLoopingSound(USoundBase* Sound) { LoopingSound = Sound; }

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
		
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	FAdditionalEffectParams AdditionalEffectParams;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;
	
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	
	void OnHit();

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;

	bool bHit = false;
	
	UPROPERTY() TObjectPtr<UNiagaraSystem> ImpactEffect;
	UPROPERTY() TObjectPtr<USoundBase> ImpactSound;
	UPROPERTY() TObjectPtr<USoundBase> LoopingSound;
	UPROPERTY() TObjectPtr<UAudioComponent> LoopingSoundComponent;
};
