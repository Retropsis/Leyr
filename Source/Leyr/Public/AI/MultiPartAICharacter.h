// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacter.h"
#include "MultiPartAICharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimationFinished);

/**
 * 
 */
UCLASS()
class LEYR_API AMultiPartAICharacter : public AAICharacter
{
	GENERATED_BODY()

public:
	AMultiPartAICharacter();
	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayHitReactMontage();

	UFUNCTION(BlueprintCallable)
	void PlayMultiPartMontage(UAnimationAsset* AnimToPlay);

	UFUNCTION(BlueprintImplementableEvent)
	void HandleOnAnimationFinished();
	
	UFUNCTION(BlueprintImplementableEvent)
	void HandleDeathMultiParts();

protected:
	virtual void MulticastHandleDeath(const FVector& DeathImpulse, EDefeatState InDefeatState) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> MultiPartFlipbook;

	UPROPERTY(EditDefaultsOnly)
	FName HeadSocket = FName("Head");
};
