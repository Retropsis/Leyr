// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilityTypes.h"
#include "Data/ProjectileData.h"
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
	virtual bool InitProjectileData();
	void SetImpactEffect(UNiagaraSystem* Effect) { ImpactEffect = Effect; }
	void SetImpactSound(USoundBase* Sound) { ImpactSound = Sound; }
	void SetLoopingSound(USoundBase* Sound) { LoopingSound = Sound; }
	bool IsValidOverlap(AActor* OtherActor);

	/*
	 * OnHit VFX Timeline
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnHitTimeline();

	UFUNCTION(BlueprintCallable)
	void PrepareToDestroyProjectile();

	UPROPERTY(BlueprintReadOnly)
	FVector InitialOnHitTimeline = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly)
	FVector TargetLocation = FVector::ZeroVector;
	
	bool bPlayOnHitTimeline = false;

	/*
	 *
	 */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UProjectileData> ProjectileData = nullptr;
	
	EResponseToStatic ResponseToStatic = EResponseToStatic::Destroy;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	FAdditionalEffectParams DamageEffectParams;
		
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	FAdditionalEffectParams StatusEffectParams;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetComponent;
	
	bool bHit = false;
	
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnHit();

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	UFUNCTION()
	void OnHomingTargetDeath(AActor* DeadActor);
	
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;
	
	UPROPERTY() TObjectPtr<UNiagaraSystem> ImpactEffect;
	UPROPERTY() TObjectPtr<USoundBase> ImpactSound;
	UPROPERTY() TObjectPtr<USoundBase> LoopingSound;
	UPROPERTY() TObjectPtr<UAudioComponent> LoopingSoundComponent;
};
