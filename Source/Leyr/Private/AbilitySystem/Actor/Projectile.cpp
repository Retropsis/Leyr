// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/Projectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Data/StatusEffectData.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/InteractionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Leyr/Leyr.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Player, ECR_Ignore);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->SetPlaneConstraintEnabled(true);
	ProjectileMovement->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->InitialSpeed = 800.f;
	ProjectileMovement->MaxSpeed = 800.f;
	ProjectileMovement->ProjectileGravityScale = 1.f;
}

bool AProjectile::InitProjectileData()
{
	if (ProjectileData)
	{
		DamageEffectParams.DamageType = ProjectileData->DamageType;
		
		Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ProjectileData->ResponseToStatic == EResponseToStatic::Ignore ? ECR_Ignore : ECR_Overlap);
		ResponseToStatic = ProjectileData->ResponseToStatic;
		ProjectileMovement->ProjectileGravityScale = ProjectileData->ProjectileGravityScale;
		ProjectileMovement->InitialSpeed = ProjectileData->InitialSpeed;
		ProjectileMovement->MaxSpeed = ProjectileData->MaxSpeed;
		bPlayOnHitTimeline = ProjectileData->bPlayOnHitTimeline;
		SetImpactEffect(ProjectileData->ImpactEffect);
		SetImpactSound(ProjectileData->ImpactSound);
		SetLoopingSound(ProjectileData->LoopingSound);
		
		if (ProjectileData->StatusEffectData)
		{
			StatusEffectParams.DamageType = ProjectileData->StatusEffectData->StatusEffectType;
			StatusEffectParams.StatusEffectChance = ProjectileData->StatusEffectData->StatusEffectChance;
			StatusEffectParams.StatusEffectDamage = ProjectileData->StatusEffectData->StatusEffectDamage;
			StatusEffectParams.StatusEffectDuration = ProjectileData->StatusEffectData->StatusEffectDuration;
			StatusEffectParams.StatusEffectFrequency = ProjectileData->StatusEffectData->StatusEffectFrequency;
		}
		return true;
	}
	return false;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	SetReplicateMovement(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnSphereOverlap);
	
	if(LoopingSoundComponent)
	{
		LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
		LoopingSoundComponent->bStopWhenOwnerDestroyed = true;
	}
	
	if (HasAuthority() && ProjectileMovement->HomingTargetComponent.IsValid())
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ProjectileMovement->HomingTargetComponent->GetOwner()))
		{
			CombatInterface->GetOnDeath().AddUniqueDynamic(this, &ThisClass::OnHomingTargetDeath);
		}
	}
}

void AProjectile::OnHomingTargetDeath(AActor* DeadActor)
{
	ProjectileMovement->bIsHomingProjectile = false;
}

bool AProjectile::IsValidOverlap(AActor* OtherActor)
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return false;
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (!IsValid(SourceAvatarActor)) Destroy();
	if (SourceAvatarActor == OtherActor) return false;
	if (!ULeyrAbilitySystemLibrary::IsHostile(SourceAvatarActor, OtherActor)) return false;
 
	return true;
}

void AProjectile::PrepareToDestroyProjectile()
{
	Sphere->SetSimulatePhysics(true);
	Sphere->SetEnableGravity(true);
	ProjectileMovement->ProjectileGravityScale = 0.3f;
}

void AProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsValidOverlap(OtherActor)) return;
	
	if (OtherComp && OtherComp->GetCollisionObjectType() == ECC_WorldStatic)
	{
		if (ResponseToStatic == EResponseToStatic::Destroy)
		{
			if (bPlayOnHitTimeline)
			{
				InitialOnHitTimeline = GetActorLocation();
				ProjectileMovement->StopMovementImmediately();
				const FVector RandomRotation = GetActorForwardVector().RotateAngleAxis(FMath::FRandRange(112.5f, 157.5f), FVector::RightVector);
				TargetLocation = GetActorLocation() + RandomRotation * FMath::FRandRange(45.f, 85.f);
				// UKismetSystemLibrary::DrawDebugSphere(this, TargetLocation, 3.f, 12, FLinearColor::Green, 3.f);
				// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), TargetLocation, 5.f, FLinearColor::Green, 3.f);
				Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
				OnHitTimeline();
			} else Destroy();
			return;
		}
		if (ResponseToStatic == EResponseToStatic::Stop)
		{
			ProjectileMovement->StopMovementImmediately();
			Sphere->OnComponentBeginOverlap.Clear();
			SetLifeSpan(5.f);
			return;
		}
	}
	//
	// if (!ULeyrAbilitySystemLibrary::IsHostile(SourceAvatarActor, OtherActor)) return;
	if (!bHit) OnHit();

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			if (FMath::RandRange(1, 100) < DamageEffectParams.AirborneChance)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;

				const FVector AirborneDirection = Rotation.Vector();
				// const FVector AirborneForce = AirborneDirection * AdditionalEffectParams.AirborneForceMagnitude;
				const FVector AirborneForce = FVector::UpVector * DamageEffectParams.AirborneForceMagnitude;
				DamageEffectParams.AirborneForce = AirborneForce;
			}
			
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			ULeyrAbilitySystemLibrary::ApplyAdditionalEffect(DamageEffectParams);
			
			StatusEffectParams.TargetAbilitySystemComponent = TargetASC;
			ULeyrAbilitySystemLibrary::ApplyStatusEffect(StatusEffectParams);
			
			Destroy();
		}
			
		if(OtherActor && OtherActor->Implements<UInteractionInterface>())
		{
			IInteractionInterface::Execute_InteractHit(OtherActor, /*SourceAvatarActor*/ GetInstigator());
		}
		bool bActorOverlappingProjectiles = OtherActor && OtherActor->ActorHasTag("OverlapProjectiles");
		bool bInteractiveActorBlockingProjectile = OtherActor && OtherActor->Implements<UInteractionInterface>() && IInteractionInterface::Execute_ShouldBlockProjectile(OtherActor);
		if(!bActorOverlappingProjectiles || bInteractiveActorBlockingProjectile)
		{
			ProjectileMovement->StopMovementImmediately();
		}
	}
	else bHit = true;
}

void AProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	bHit = true;
}

void AProjectile::Destroyed()
{
	if (!bHit && !HasAuthority()) OnHit();
	Super::Destroyed();
}

