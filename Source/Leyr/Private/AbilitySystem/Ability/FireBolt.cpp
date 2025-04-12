// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/FireBolt.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystem/Actor/Projectile.h"
#include "Data/AbilityData.h"
#include "GameFramework/ProjectileMovementComponent.h"

FString UFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = AbilityPower.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Launches a bolt of fire, "
			"exploding on impact and dealing: </>"
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"), Level, ManaCost, Cooldown, ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Launches %d bolts of fire, "
			"exploding on impact and dealing: </>"
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"), Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), ScaledDamage);		
	}
}

FString UFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = AbilityPower.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL: </>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Launches %d bolts of fire, "
			"exploding on impact and dealing: </>"
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"), Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), ScaledDamage);
}

void UFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, AActor* HomingTarget)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
 
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = bHasTarget ? (ProjectileTargetLocation - SocketLocation).Rotation() : GetAvatarActorFromActorInfo()->GetActorForwardVector().Rotation();
	// FRotator Rotation = bHasTarget ? UKismetMathLibrary::FindLookAtRotation(SocketLocation, ProjectileTargetLocation) : GetAvatarActorFromActorInfo()->GetActorForwardVector().Rotation();
	// FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchOverride;
	
	const int32 EffectiveNumProjectiles = FMath::Min(NumProjectiles, GetAbilityLevel());
	
	const FVector Forward = Rotation.Vector();
	TArray<FRotator> Rotations =  ULeyrAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::RightVector, ProjectileSpread, EffectiveNumProjectiles);

	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());
	
		AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
				AbilityData->ProjectileClass, SpawnTransform,
				GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()),
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
		Projectile->DamageEffectParams = MakeAdditionalEffectParamsFromClassDefaults();
		Projectile->StatusEffectParams = MakeStatusEffectParamsFromClassDefaults();
		Projectile->InitProjectileData();

		if (bLaunchHomingProjectiles)
		{
			Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
			Projectile->ProjectileMovement->bIsHomingProjectile = true;
			if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
			{
				Projectile->ProjectileMovement->bIsHomingProjectile = true;
				Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
			}
			else
			{
				Projectile->HomingTargetComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
				Projectile->HomingTargetComponent->SetWorldLocation(ProjectileTargetLocation);
				Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetComponent;
			}
		}
		Projectile->FinishSpawning(SpawnTransform);
	}
}
