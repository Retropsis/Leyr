// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/Projectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/InteractionInterface.h"
#include "Kismet/GameplayStatics.h"
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
}

void AProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if(!IsValidOverlap(OtherActor)) return;
    // if (OtherActor == GetInstigator()) return;
	
	// if (!DamageEffectSpecHandle.Data.IsValid() || DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor) return;
	// if (!ULeyrAbilitySystemLibrary::IsHostile(DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(), OtherActor)) return;

	AActor* SourceAvatarActor = AdditionalEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (!IsValid(SourceAvatarActor)) Destroy();
	if (SourceAvatarActor == OtherActor) return;

	if (OtherComp && OtherComp->GetCollisionObjectType() == ECC_WorldStatic)
	{
		if (ResponseToStatic == EResponseToStatic::Destroy)
		{
			Destroy();
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
	
	if (!ULeyrAbilitySystemLibrary::IsHostile(SourceAvatarActor, OtherActor)) return;
	if (!bHit) OnHit();

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * AdditionalEffectParams.DeathImpulseMagnitude;
			AdditionalEffectParams.DeathImpulse = DeathImpulse;
			if (FMath::RandRange(1, 100) < AdditionalEffectParams.AirborneChance)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;

				const FVector AirborneDirection = Rotation.Vector();
				// const FVector AirborneForce = AirborneDirection * AdditionalEffectParams.AirborneForceMagnitude;
				const FVector AirborneForce = FVector::UpVector * AdditionalEffectParams.AirborneForceMagnitude;
				AdditionalEffectParams.AirborneForce = AirborneForce;
			}
			AdditionalEffectParams.TargetAbilitySystemComponent = TargetASC;
			ULeyrAbilitySystemLibrary::ApplyAdditionalEffect(AdditionalEffectParams);
			Destroy();
		}
			
		if(OtherActor && OtherActor->Implements<UInteractionInterface>())
		{
			IInteractionInterface::Execute_InteractHit(OtherActor, SourceAvatarActor);
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

// bool AProjectile::IsValidOverlap(AActor* OtherActor)
// {
// 	if(!IsValid(DamageEffectParams.SourceAbilitySystemComponent)) return false;
// 	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
// 	if(SourceAvatarActor == OtherActor) return false;
// 	if(!UBaseAbilitySystemLibrary::IsHostile(SourceAvatarActor, OtherActor)) return false;
//
// 	return true;
// }

void AProjectile::Destroyed()
{
	if (!bHit && !HasAuthority()) OnHit();
	Super::Destroyed();
}

