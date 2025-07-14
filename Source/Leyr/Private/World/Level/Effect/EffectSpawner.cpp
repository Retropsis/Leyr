// @ Retropsis 2024-2025.

#include "World/Level/Effect/EffectSpawner.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AEffectSpawner::AEffectSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEffectSpawner::SpawnEffects(UPrimitiveComponent* OverlappedComponent, const FHitResult& HitResult)
{
	if (OverlappedComponent == nullptr) return;
	if (SpawnFrequency == EEffectSpawnFrequency::Roll && FMath::RandRange(0, 99) <= SpawnChance) return;
	
	const int32 NiagaraSelection = FMath::RandRange(0, NiagaraEffects.Num() - 1);
	const int32 SoundSelection = FMath::RandRange(0, SoundEffects.Num() - 1);
	const FVector SpawnLocation = FVector{ HitResult.ImpactPoint.X, 5.0f, OverlappedComponent->GetComponentLocation().Z };
	
	switch (SpawnRule)
	{
	case EEffectSpawnRule::Random:
			if (!NiagaraEffects.IsEmpty()) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, NiagaraEffects[NiagaraSelection], SpawnLocation);
			if (!SoundEffects.IsEmpty()) UGameplayStatics::PlaySoundAtLocation(this, SoundEffects[SoundSelection], SpawnLocation);
			break;
		case EEffectSpawnRule::All:
			for (TObjectPtr<UNiagaraSystem> Effect : NiagaraEffects)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Effect, SpawnLocation);
			}
			for (TObjectPtr<USoundBase> Effect : SoundEffects)
			{
				UGameplayStatics::PlaySoundAtLocation(this, Effect, SpawnLocation);
			}
			break;
	}
}
