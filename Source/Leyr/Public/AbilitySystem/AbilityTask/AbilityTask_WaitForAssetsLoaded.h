// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitForAssetsLoaded.generated.h"

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FRequestedRelevantAssetsData
{
	GENERATED_BODY()
 
	UPROPERTY(BlueprintReadOnly)
	TArray<TSoftObjectPtr<UObject>> AssetsLoaded;
 
	FRequestedRelevantAssetsData()	{}
	FRequestedRelevantAssetsData(const TArray<TSoftObjectPtr<UObject>>& InAssets)
		:AssetsLoaded(InAssets)
	{}
};
 
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestedRelevantAssetsEvent, const FRequestedRelevantAssetsData&, RequestedRelevantAssetsData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestedRelevantAssetsEvent, const FRequestedRelevantAssetsData, RequestedAssets);
 
/**
 * 
 */
UCLASS()
class LEYR_API UAbilityTask_WaitForAssetsLoaded : public UAbilityTask
{
	GENERATED_BODY()
 
	UPROPERTY(BlueprintAssignable)
	FRequestedRelevantAssetsEvent OnAssetsLoaded;
 
	UPROPERTY(BlueprintAssignable)
	FRequestedRelevantAssetsEvent OnAssetsFailedToLoad;
 
	//Task Factory function
	UFUNCTION(BlueprintCallable, meta=(HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Ability|Tasks")
	static UAbilityTask_WaitForAssetsLoaded* WaitForAssetsLoaded(UGameplayAbility* OwningAbility, const TArray<TSoftObjectPtr<UObject>> InAssetsToStream);

	virtual void Activate() override;
	void AssetsLoaded() const;

protected:
	UPROPERTY()
	TArray<FSoftObjectPath> AssetsToStream;
	
	UPROPERTY()
	TArray<TSoftObjectPtr<UObject>> LoadedAssets;
};
