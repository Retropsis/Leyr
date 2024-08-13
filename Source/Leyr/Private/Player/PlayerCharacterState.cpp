// @ Retropsis 2024-2025.

#include "Player/PlayerCharacterState.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Net/UnrealNetwork.h"

APlayerCharacterState::APlayerCharacterState()
{
	NetUpdateFrequency = 100.f;
	
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
}

void APlayerCharacterState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APlayerCharacterState, Level);
}

/*
 * Ability System
 */
UAbilitySystemComponent* APlayerCharacterState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APlayerCharacterState::OnRep_Level(int32 OldLevel)
{
}
