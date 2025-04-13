// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/Boomerang.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

ABoomerang::ABoomerang()
{
	ProjectileMovement->PrimaryComponentTick.bCanEverTick = true;
	ProjectileMovement->SetAutoActivate(true);

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>("RotatingMovementComponent");
	RotatingMovementComponent->RotationRate = FRotator{ RotationRate, 0.f, 0.f };
	RotatingMovementComponent->bRotationInLocalSpace = true;
}

void ABoomerang::OnActivated(UActorComponent* Component, bool bReset)
{
	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Green, FString::Printf(TEXT("Activated with Velocity %s"), *ProjectileMovement->Velocity.ToCompactString()));
}

void ABoomerang::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
	ApexLocation = InitialLocation + GetActorForwardVector() * TravelDistance;
	InitialDirection = GetActorForwardVector();
	StartOutgoingTimeline();
	ProjectileMovement->Deactivate();
	ProjectileMovement->OnComponentActivated.AddDynamic(this, &ABoomerang::OnActivated);
}

void ABoomerang::OnHit()
{
	Destroy();
	// if (GetOwner())
	// {
	// 	FGameplayCueParameters CueParams;
	// 	CueParams.Location = GetActorLocation();
	// 	UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FBaseGameplayTags::Get().GameplayCue_Weapon, CueParams);
	// }
	bHit = true;
}

void ABoomerang::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && GetInstigator() == OtherActor) Destroy();
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

void ABoomerang::OutgoingTimelineEnd()
{
	ProjectileMovement->InitialSpeed = 600.f;
	ProjectileMovement->MaxSpeed = 600.f;
	ProjectileMovement->SetUpdatedComponent(GetRootComponent());
	ProjectileMovement->Velocity = InitialDirection * -1.f * ProjectileMovement->InitialSpeed;
	ProjectileMovement->Activate();
}
