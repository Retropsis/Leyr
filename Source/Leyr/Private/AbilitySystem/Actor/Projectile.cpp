// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/Projectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
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

	GhostTrail = CreateDefaultSubobject<UNiagaraComponent>("GhostTrail");
	GhostTrail->SetupAttachment(Sphere);
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
		if (ProjectileData->bRandomResponseToStatic)
		{
			ResponseToStatic = FMath::RandRange(0,1) == 0 ? EResponseToStatic::Destroy : EResponseToStatic::Stop;
			bPlayOnHitTimeline = ResponseToStatic == EResponseToStatic::Destroy;
		}
		SetImpactEffect(ProjectileData->ImpactEffect);
		SetGhostTrailEffect(ProjectileData->GhostTrailEffect);
		SetImpactSound(ProjectileData->ImpactSound);
		SetLoopingSound(ProjectileData->LoopingSound);
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

	if (GhostTrailEffect)
	{
		GhostTrail = UNiagaraFunctionLibrary::SpawnSystemAttached(GhostTrailEffect, Sphere, FName(), FVector::ZeroVector, FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset, true, true);
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

bool AProjectile::IsValidOverlap(const AActor* OtherActor)
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return false;
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (!IsValid(SourceAvatarActor))
	{
		Destroy();
		return false;
	}
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
		GhostTrail->DeactivateImmediate();
		if (ResponseToStatic == EResponseToStatic::Destroy)
		{
			if (bPlayOnHitTimeline)
			{
				InitialOnHitTimeline = GetActorLocation();
				ProjectileMovement->StopMovementImmediately();
				const FVector RandomRotation = GetActorForwardVector().RotateAngleAxis(FMath::FRandRange(112.5f, 157.5f), FVector::RightVector);
				TargetLocation = GetActorLocation() + RandomRotation * FMath::FRandRange(45.f, 85.f);
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
			
			if (ProjectileData->StatusEffectData)
			{
				UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
				ProjectileData->StatusEffectData->ApplyStatusEffect(SourceASC, TargetASC);
			}
			
			Destroy();
		}
			
		if(OtherActor && OtherActor->Implements<UInteractionInterface>())
		{
			IInteractionInterface::Execute_InteractHit(OtherActor, /*SourceAvatarActor*/ GetInstigator(), SweepResult.BoneName);
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

