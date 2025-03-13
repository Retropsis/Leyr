// @ Retropsis 2024-2025.

#include "AbilitySystem/AbilityTask/AbilityTask_WaitForAssetLoaded.h"

#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "AbilitySystem/Cue/GameplayCueDefinition.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

UAbilityTask_WaitForAssetLoaded* UAbilityTask_WaitForAssetLoaded::CreateWaitForAssetLoaded(UGameplayAbility* OwningAbility, const TSoftObjectPtr<USoundBase> ImpactSound)
{
	UAbilityTask_WaitForAssetLoaded* NewTask = NewAbilityTask<UAbilityTask_WaitForAssetLoaded>(OwningAbility);

	if (const UBaseGameplayAbility* BaseGameplayAbility = Cast<UBaseGameplayAbility>(OwningAbility))
	{
		NewTask->GameplayCueDefinition = BaseGameplayAbility->GetGameplayCueDefinition();
		NewTask->ImpactSound = ImpactSound;
	}
	
	return NewTask;
}

void UAbilityTask_WaitForAssetLoaded::Activate()
{
	UAssetManager::GetStreamableManager().RequestAsyncLoad(ImpactSound.ToSoftObjectPath(), [this] () {
		USoundBase* LoadedAsset = ImpactSound.Get();
		if (IsValid(LoadedAsset))
		{
			GameplayCueDefinition->ImpactSound = LoadedAsset;
			AssetLoaded.Broadcast();
		}
	}, FStreamableManager::AsyncLoadHighPriority);
}
