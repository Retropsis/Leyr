// @ Retropsis 2024-2025.

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "World/Item/AttributeUpgrade.h"

AAttributeUpgrade::AAttributeUpgrade()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAttributeUpgrade::Interact_Implementation(AActor* InteractingActor)
{
	Super::Interact_Implementation(InteractingActor);
	UBaseAbilitySystemComponent* BaseASC = CastChecked<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InteractingActor));
	BaseASC->ServerUpgradeAttributeFromItem(AttributeTag);
}
