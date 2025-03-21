// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/ProjectileAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Actor/Projectile.h"
#include "Components/SphereComponent.h"
#include "Data/AbilityData.h"
#include "Data/ProjectileData.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UProjectileAbility::InitAbility()
{
	Super::InitAbility();
	
	if (AbilityData)
	{
		bOverridePitch = AbilityData->bOverridePitch;
		PitchOverride = AbilityData->PitchOverride;
	}
}

void UProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UProjectileAbility::SpawnProjectile(const FGameplayTag& SocketTag, bool bHasTarget, const FVector& ProjectileTargetLocation)
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
	
	if(AbilityData->ProjectileData)
	{		
		AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
			AbilityData->ProjectileData->ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
		Projectile->AdditionalEffectParams = MakeAdditionalEffectParamsFromClassDefaults();
		Projectile->AdditionalEffectParams.DamageType = AbilityData->ProjectileData->DamageType;
		Projectile->Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, AbilityData->ProjectileData->ResponseToStatic == EResponseToStatic::Ignore ? ECR_Ignore : ECR_Overlap);
		Projectile->ResponseToStatic = AbilityData->ProjectileData->ResponseToStatic;
		Projectile->ProjectileMovement->ProjectileGravityScale = AbilityData->ProjectileData->ProjectileGravityScale;
		Projectile->ProjectileMovement->InitialSpeed = AbilityData->ProjectileData->InitialSpeed;
		Projectile->ProjectileMovement->MaxSpeed = AbilityData->ProjectileData->MaxSpeed;
		Projectile->SetImpactEffect(AbilityData->ProjectileData->ImpactEffect);
		Projectile->SetImpactSound(AbilityData->ProjectileData->ImpactSound);
		Projectile->SetLoopingSound(AbilityData->ProjectileData->LoopingSound);
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}

void UProjectileAbility::SetPitchOverride(bool bShouldOverride, const float Pitch)
{
	bOverridePitch = bShouldOverride;
	PitchOverride = Pitch;
}