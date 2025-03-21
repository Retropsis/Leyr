// @ Retropsis 2024-2025.

#include "AbilitySystem/AbilityTask/AbilityTask_WaitForAssetsLoaded.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

UAbilityTask_WaitForAssetsLoaded* UAbilityTask_WaitForAssetsLoaded::WaitForAssetsLoaded(UGameplayAbility* OwningAbility, const TArray<TSoftObjectPtr<UObject>> InAssetsToStream)
{
	UAbilityTask_WaitForAssetsLoaded* MyObj = NewAbilityTask<UAbilityTask_WaitForAssetsLoaded>(OwningAbility);
	
	for (TSoftObjectPtr<UObject> RequestedObject : InAssetsToStream)
	{
		MyObj->AssetsToStream.AddUnique(RequestedObject.ToSoftObjectPath());
	}
	
	return MyObj;
}

void UAbilityTask_WaitForAssetsLoaded::Activate()
{
	if (AssetsToStream.Num() <= 0)
	{
		OnAssetsFailedToLoad.Broadcast(FRequestedRelevantAssetsData());
		return;
	}
	UAssetManager::GetStreamableManager().RequestAsyncLoad(AssetsToStream, FStreamableDelegate::CreateUObject(this, &UAbilityTask_WaitForAssetsLoaded::AssetsLoaded));
}

void UAbilityTask_WaitForAssetsLoaded::AssetsLoaded() const
{
	TArray<TSoftObjectPtr<UObject>> AssetsLoaded;
	for (FSoftObjectPath LoadedObject : AssetsToStream)
	{
		AssetsLoaded.AddUnique(LoadedObject.ResolveObject());
	}
	FRequestedRelevantAssetsData Data;
	Data.AssetsLoaded = AssetsLoaded;
	OnAssetsLoaded.Broadcast(Data);
}