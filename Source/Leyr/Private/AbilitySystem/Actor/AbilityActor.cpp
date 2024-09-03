// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/AbilityActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

AAbilityActor::AAbilityActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAbilityActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		InitializeDefaultAttributes();		
	}
}

void AAbilityActor::InitializeDefaultAttributes() const
{
	// ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	// ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	// ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
	ULeyrAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAbilityActor::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpan);
	bDead = true;
	MulticastHandleDeath(DeathImpulse);
}

void AAbilityActor::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	UGameplayStatics::PlaySoundAtLocation(this, DestructionSound, GetActorLocation(), GetActorRotation());
	bDead = true;
	OnDeath.Broadcast(this);
}
