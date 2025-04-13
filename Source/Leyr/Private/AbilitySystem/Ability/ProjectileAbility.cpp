// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/ProjectileAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Actor/Projectile.h"
#include "Data/AbilityData.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"

void UProjectileAbility::InitAbility()
{
	Super::InitAbility();
	
	if (AbilityData)
	{
		bOverridePitch = AbilityData->bOverridePitch;
		PitchOverride = AbilityData->PitchOverride;
		ProjectileSpread =AbilityData->ProjectileSpread;
		MaxNumProjectiles = AbilityData->MaxNumProjectiles;
		bLaunchHomingProjectiles = AbilityData->bLaunchHomingProjectiles;
		HomingAccelerationMin = AbilityData->HomingAccelerationMin;
		HomingAccelerationMax = AbilityData->HomingAccelerationMax;
	}
}

void UProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UProjectileAbility::SpawnProjectile(const FGameplayTag& SocketTag, const FVector& ProjectileTargetLocation)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = bHasTarget ? UKismetMathLibrary::FindLookAtRotation(SocketLocation, ProjectileTargetLocation) : GetAvatarActorFromActorInfo()->GetActorForwardVector().Rotation();
	// FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
			AbilityData->ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController.Get() ? CurrentActorInfo->PlayerController->GetPawn() : Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->DamageEffectParams = MakeAdditionalEffectParamsFromClassDefaults();
	Projectile->SetOwner(GetAvatarActorFromActorInfo());
	Projectile->InitProjectileData();
	Projectile->FinishSpawning(SpawnTransform);
}

void UProjectileAbility::SetPitchOverride(bool bShouldOverride, const float Pitch)
{
	bOverridePitch = bShouldOverride;
	PitchOverride = Pitch;
}