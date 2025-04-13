// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/FireBall.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "GameplayCueManager.h"
#include "Game/BaseGameplayTags.h"
#include "GameFramework/ProjectileMovementComponent.h"

AFireBall::AFireBall()
{
	ProjectileMovement->PrimaryComponentTick.bCanEverTick = false;
	ProjectileMovement->SetAutoActivate(false);
}

void AFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AFireBall::OnHit()
{
	if (GetOwner())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FBaseGameplayTags::Get().GameplayCue_FireBlast, CueParams);
	}
	bHit = true;
}

void AFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
 
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			ULeyrAbilitySystemLibrary::ApplyAdditionalEffect(DamageEffectParams);
		}
	}
}
