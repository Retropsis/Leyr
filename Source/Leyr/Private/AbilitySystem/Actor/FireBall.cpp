// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/FireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
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

void AFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
 
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * AdditionalEffectParams.DeathImpulseMagnitude;
			AdditionalEffectParams.DeathImpulse = DeathImpulse;
			AdditionalEffectParams.TargetAbilitySystemComponent = TargetASC;
			ULeyrAbilitySystemLibrary::ApplyAdditionalEffect(AdditionalEffectParams);
		}
	}
}
