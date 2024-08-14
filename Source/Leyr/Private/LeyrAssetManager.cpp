// @ Retropsis 2024-2025.

#include "LeyrAssetManager.h"
#include "Game/BaseGameplayTags.h"

ULeyrAssetManager& ULeyrAssetManager::Get()
{
	check(GEngine);
	ULeyrAssetManager* LeyrAssetManager = Cast<ULeyrAssetManager>(GEngine->AssetManager);
	return *LeyrAssetManager;
}

void ULeyrAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FBaseGameplayTags::InitializeNativeGameplayTags();
}
