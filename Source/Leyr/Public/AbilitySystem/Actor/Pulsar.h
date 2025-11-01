// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Actor/Projectile.h"
#include "Pulsar.generated.h"

class ACluster;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class LEYR_API APulsar : public AProjectile
{
	GENERATED_BODY()

public:
	APulsar();

protected:
	virtual void BeginPlay() override;

	UFUNCTION() void OnPulse();
	UFUNCTION() void OnSweep(const FVector& TraceStart);
	UFUNCTION() void CauseDamage(AActor* OtherActor);
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ACluster> ClusterClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> PulsingEffect;

	FTimerHandle PulsingTimer;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PulsingTime = .5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SweepDelay = .5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TraceRadius = 25.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector StartOffset = FVector(0.f, 0.f, -30.f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector EndOffset = FVector(0.f, 0.f, 100.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDebug = false;
};
