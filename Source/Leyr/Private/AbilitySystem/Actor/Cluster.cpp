// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/Cluster.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Data/StatusEffectData.h"
#include "Game/BaseGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/InteractionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Leyr/Leyr.h"

ACluster::ACluster()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	HitCollision = CreateDefaultSubobject<UBoxComponent>(FName("HitCollision"));
	SetRootComponent(HitCollision);
	HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitCollision->SetRelativeLocation(FVector(0, 0, 55.f ));
	HitCollision->SetBoxExtent(FVector{ 22.f, 100.f, 50.f });
}

void ACluster::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	
	if (HasAuthority())
	{
		HitCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCollisionOverlap);
	}
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&] ()
	{
		EffectStart();
	}, EffectDelay, false);
}

void ACluster::EffectStart() const
{
	HitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitCollision->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	HitCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&] ()
	{
		EffectEnded();
	}, EffectiveTime, false);
}

void ACluster::EffectEnded() const
{
	if (!IsValid(HitCollision)) return;
	
	HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitCollision->SetCollisionResponseToAllChannels(ECR_Ignore);	
}

void ACluster::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor->Implements<UCombatInterface>())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		CauseDamage(OtherActor);
		EffectEnded();
	}
}

void ACluster::InitializeCluster(const FAdditionalEffectParams& Params, UStatusEffectData* EffectData, UNiagaraSystem* Effect, USoundBase* Sound)
{
	DamageEffectParams = Params;
	StatusEffectData = EffectData;
	ImpactEffect = Effect;
	ImpactSound = Sound;
}

void ACluster::CauseDamage(AActor* OtherActor)
{
	if (!IsValid(DamageEffectParams.SourceAbilitySystemComponent)) return;
	
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			if(TargetASC && TargetASC->HasMatchingGameplayTag(FBaseGameplayTags::Get().Invincibility)) return;
			
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
			ULeyrAbilitySystemLibrary::ApplyInvincibilityToTarget(TargetASC, 1.25f);
			
			if (StatusEffectData.IsValid())
			{
				UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
				StatusEffectData->ApplyStatusEffect(SourceASC, TargetASC);
			}
		}
			
		if(OtherActor && OtherActor->Implements<UInteractionInterface>())
		{
			IInteractionInterface::Execute_InteractHit(OtherActor, /*SourceAvatarActor*/ GetInstigator(), /*SweepResult.BoneName*/ FName());
		}
	}
}

