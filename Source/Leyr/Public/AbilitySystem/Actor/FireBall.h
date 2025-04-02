// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Actor/Projectile.h"
#include "FireBall.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API AFireBall : public AProjectile
{
	GENERATED_BODY()
	
public:
	AFireBall();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TravelDistance = 1600.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ExplodeDistance = 75.f;
	
	UPROPERTY(BlueprintReadWrite)
	FAdditionalEffectParams ExplosionDamageParams;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnHit() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
