// @ Retropsis 2024-2025.

#include "AbilitySystem/Effect/StatusEffectNiagaraComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interaction/CombatInterface.h"

UStatusEffectNiagaraComponent::UStatusEffectNiagaraComponent()
{
	bAutoActivate = false;
}

void UStatusEffectNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->RegisterGameplayTagEvent(StatusEffectTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UStatusEffectNiagaraComponent::StatusEffectTagChanged);
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnASCRegistered().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
		{
			InASC->RegisterGameplayTagEvent(StatusEffectTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UStatusEffectNiagaraComponent::StatusEffectTagChanged);
		});
	}
	if (CombatInterface)
	{
		CombatInterface->GetOnDeath().AddDynamic(this, &UStatusEffectNiagaraComponent::OnOwnerDeath);
	}
}

void UStatusEffectNiagaraComponent::StatusEffectTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const bool bOwnerAlive = IsValid(GetOwner()) && GetOwner()->Implements<UCombatInterface>() && ICombatInterface::Execute_GetDefeatState(GetOwner()) == EDefeatState::None;
	NewCount > 0 && bOwnerAlive ? Activate() : DeactivateImmediate();
}
