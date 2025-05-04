// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "MultiPartComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class LEYR_API UMultiPartComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void AttackComponentsToSkeleton(TArray<USceneComponent*> Components);
};
