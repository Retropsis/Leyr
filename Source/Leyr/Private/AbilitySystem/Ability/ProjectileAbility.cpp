// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/ProjectileAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Actor/Projectile.h"
#include "Components/SphereComponent.h"
#include "Data/AbilityData.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"

void UProjectileAbility::InitAbility()
{
	Super::InitAbility();
	
	if (AbilityData)
	{
		ProjectileClass = AbilityData->ProjectileClass;
		bIgnoreStatic = AbilityData->bIgnoreStatic;
		bOverridePitch = AbilityData->bOverridePitch;
		PitchOverride = AbilityData->PitchOverride;
	}
}

void UProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UProjectileAbility::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool InIgnoreStatic, bool InOverridePitch, float InPitchOverride)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(SocketLocation, ProjectileTargetLocation);
	// FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (InOverridePitch)
	{
		Rotation.Pitch = InPitchOverride;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	Projectile->AdditionalEffectParams = MakeAdditionalEffectParamsFromClassDefaults();
	Projectile->AdditionalEffectParams.DamageType = DamageType;
	Projectile->Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, InIgnoreStatic ? ECR_Ignore : ECR_Block);
	Projectile->FinishSpawning(SpawnTransform);
}
