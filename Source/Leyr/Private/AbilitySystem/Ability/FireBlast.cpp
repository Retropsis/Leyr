// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/FireBlast.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystem/Actor/FireBall.h"
#include "Data/AbilityData.h"

TArray<AFireBall*> UFireBlast::SpawnFireBalls()
{
	TArray<AFireBall*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = ULeyrAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::RightVector, 360.f, MaxNumProjectiles);
 
	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
 		
		AFireBall* FireBall = GetWorld()->SpawnActorDeferred<AFireBall>(AbilityData->ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
 		
		FireBall->AdditionalEffectParams = MakeAdditionalEffectParamsFromClassDefaults(); // Overlapping Damage
		FireBall->ExplosionDamageParams = MakeAdditionalEffectParamsFromClassDefaults(); // Exploding Damage
		FireBall->SetOwner(GetAvatarActorFromActorInfo());
		FireBall->InitProjectileData();
		FireBalls.Add(FireBall);
		FireBall->FinishSpawning(SpawnTransform);
	}
	return FireBalls;
}

FString UFireBlast::GetDescription(int32 Level)
{
	const int32 ScaledDamage = AbilityPower.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>FIRE BLAST</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Launches %d </>"
			"<Default>fire balls in all directions, each coming back and </>"
			"<Default>exploding upon return, causing </>"
			"<Damage>%d</><Default> radial fire damage with"
			" a chance to burn</>"),
			Level, ManaCost, Cooldown, MaxNumProjectiles, ScaledDamage);
}

FString UFireBlast::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = AbilityPower.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL:</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Launches %d </>"
			"<Default>fire balls in all directions, each coming back and </>"
			"<Default>exploding upon return, causing </>"
			"<Damage>%d</><Default> radial fire damage with"
			" a chance to burn</>"),
			Level, ManaCost, Cooldown, MaxNumProjectiles, ScaledDamage);
}
