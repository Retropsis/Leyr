// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/AbilityActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AAbilityActor::AAbilityActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
}

void AAbilityActor::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
}

void AAbilityActor::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	if (HasAuthority())
	{
		InitializeDefaultAttributes();		
	}
	// OnASCRegistered.Broadcast(AbilitySystemComponent);
}

void AAbilityActor::InitializeDefaultAttributes() const
{
	ULeyrAbilitySystemLibrary::InitializeActorAttributes(this, ActorClass, Level, AbilitySystemComponent);
}

void AAbilityActor::DestroyActor_Implementation()
{
	SetLifeSpan(LifeSpan);
	bDestroyed = true;
	MulticastHandleDestruction();
}

void AAbilityActor::MulticastHandleDestruction_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, DestructionSound, GetActorLocation(), GetActorRotation());
	bDestroyed = true;
	OnDeath.Broadcast(this);
}
