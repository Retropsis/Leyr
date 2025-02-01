// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

// TODO: Could do it for GrantedTags as well ?
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, int32 /*AbilityLevel*/);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*Status*/, const FGameplayTag& /*Slot*/, const FGameplayTag& /*PrevSlot*/);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAbilityCostCommitted, const FGameplayTag&, AbilityTag, const FGameplayTag&, CostTag, int32, Quantity);

/**
 * 
 */
UCLASS()
class LEYR_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void AbilityActorInfoSet();
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	void AddCharacterCommonAbilities(const TArray<TSubclassOf<UGameplayAbility>>& CommonAbilities);
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);
	
	void UpdateAbilityStatuses(int32 Level);
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
	
	UFUNCTION(Server, Reliable)
	void ServerSpendSkillPoint(const FGameplayTag& AbilityTag);
	
	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Slot);

	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);
	
	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);
	
	void ClearSlot(FGameplayAbilitySpec* Spec);
	void ClearAbilitiesOfSlot(const FGameplayTag& Slot);
	static bool AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot);
	
	FEffectAssetTags EffectAssetTags;
	FAbilityStatusChanged AbilityStatusChanged;
	FAbilitiesGiven AbilitiesGivenDelegate;
	FAbilityEquipped AbilityEquipped;

	UPROPERTY(BlueprintAssignable)
	FAbilityCostCommitted AbilityCostCommitted;
	
	bool bStartupAbilitiesGiven = false;
	
protected:
	virtual void OnRep_ActivateAbilities() override;
	
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) const;
	
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
};
