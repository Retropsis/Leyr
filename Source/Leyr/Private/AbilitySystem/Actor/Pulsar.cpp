// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/Pulsar.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystem/Actor/Cluster.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Data/StatusEffectData.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/InteractionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Leyr/Leyr.h"

APulsar::APulsar()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void APulsar::BeginPlay()
{
	AActor::BeginPlay();
	
	SetLifeSpan(LifeSpan);
	SetReplicateMovement(true);
	
	if(LoopingSoundComponent)
	{
		LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
		LoopingSoundComponent->bStopWhenOwnerDestroyed = true;
	}
	
	// if (HasAuthority() && ProjectileMovement->HomingTargetComponent.IsValid())
	// {
	// 	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ProjectileMovement->HomingTargetComponent->GetOwner()))
	// 	{
	// 		CombatInterface->GetOnDeath().AddUniqueDynamic(this, &ThisClass::OnHomingTargetDeath);
	// 	}
	// }

	GetWorld()->GetTimerManager().SetTimer(PulsingTimer, this, &APulsar::OnPulse, PulsingTime, true);
}

void APulsar::OnPulse()
{
	if (!HasAuthority()) return;
		
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PulsingEffect, GetActorLocation() + StartOffset);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ACluster* Cluster = GetWorld()->SpawnActor<ACluster>(ClusterClass, GetActorLocation() + StartOffset, FRotator::ZeroRotator, SpawnParameters);
	Cluster->InitializeCluster(DamageEffectParams, ProjectileData->StatusEffectData, ImpactEffect, ImpactSound);
	
	FTimerHandle SweepTimer;
	FVector TraceStart = GetActorLocation() + StartOffset;
	GetWorld()->GetTimerManager().SetTimer(SweepTimer, FTimerDelegate::CreateLambda([this, TraceStart]
	{
		// OnSweep(TraceStart);
	}), SweepDelay, false);
}

void APulsar::OnSweep(const FVector& TraceStart)
{
	if (GetOwner() == nullptr) return;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte<EObjectTypeQuery> OT = GetOwner()->ActorHasTag(FName("Player")) ? EOT_EnemyCapsule : ObjectTypeQuery3;
	ObjectTypes.Add(OT);
	// ObjectTypes.Add(ICombatInterface::Execute_GetTraceObjectType(GetOwner()));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	const FVector End = TraceStart + EndOffset;
	
	FHitResult Hit;
	UKismetSystemLibrary::SphereTraceSingleForObjects(
		this, TraceStart, End, TraceRadius, ObjectTypes,
		false, ActorsToIgnore, bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, Hit, true);

	if (Hit.bBlockingHit && Hit.GetActor() && Hit.GetActor()->Implements<UCombatInterface>())
	{
		// IInteractionInterface::Execute_InteractHit(HitResult.GetActor(), GetAvatarActorFromActorInfo(), Socket);
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		CauseDamage(Hit.GetActor());
	}
	
}

void APulsar::CauseDamage(AActor* OtherActor)
{
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
		}
			
		if(OtherActor && OtherActor->Implements<UInteractionInterface>())
		{
			IInteractionInterface::Execute_InteractHit(OtherActor, /*SourceAvatarActor*/ GetInstigator(), /*SweepResult.BoneName*/ FName());
		}
	}
}
