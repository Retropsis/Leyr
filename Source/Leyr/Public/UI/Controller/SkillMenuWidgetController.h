// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Game/BaseGameplayTags.h"
#include "UI/Controller/WidgetController.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "SkillMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSkillSlotSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillSlotReassignedSignature, const FGameplayTag&, AbilityTag);

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

struct FPassiveEffect
{
	TArray<FGameplayModifierInfo> Modifiers;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class LEYR_API USkillMenuWidgetController : public UWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SkillPointsChanged;
	
	UPROPERTY(BlueprintAssignable)
	FSkillSlotSelectedSignature SkillSlotSelectedDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitingForEquipDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FSkillSlotReassignedSignature SkillSlotReassignedDelegate;

	UFUNCTION(BlueprintCallable)
	void SkillSlotSelected(const FGameplayTag& AbilityTag);
	
	UFUNCTION(BlueprintCallable)
	void SkillSlotDeselect();
	
	UFUNCTION(BlueprintCallable)
	bool SpendPointButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void SkillRowSlotPressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);
	
	UFUNCTION(BlueprintCallable)
	void ActivateButtonPressed(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	TArray<FBaseAbilityInfo> GetAvailableAbilities(const FGameplayTagContainer& Filters);

private:
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SkillPoints, bool& bShouldEnableSkillPointsButton, bool& bShouldEnableEquipButton);
	FSelectedAbility SelectedAbility = { FBaseGameplayTags::Get().Abilities_None,  FBaseGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoints = 0;
	bool bWaitingForEquipSelection = false;
	FGameplayTag SelectedSlot;

	TMap<FGameplayTag, FPassiveEffect> ActivatedEffects;
	FActiveGameplayEffectHandle ActiveEquipmentEffectHandle;
};
