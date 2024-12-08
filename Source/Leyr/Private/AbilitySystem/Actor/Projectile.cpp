// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
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

	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(5.f);
	ProjectileMovement->StopMovementImmediately();

	const AActor* SourceAvatarActor = AdditionalEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return;
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

