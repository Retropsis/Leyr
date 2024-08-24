// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Game/BaseGameplayTags.h"
#include "UI/Controller/WidgetController.h"
#include "SkillMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSkillSlotSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled);

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
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

	UFUNCTION(BlueprintCallable)
	void SkillSlotSelected(const FGameplayTag& AbilityTag);
	
	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

private:
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SkillPoints, bool& bShouldEnableSkillPointsButton, bool& bShouldEnableEquipButton);
	FSelectedAbility SelectedAbility = { FBaseGameplayTags::Get().Abilities_None,  FBaseGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoints = 0;
};
