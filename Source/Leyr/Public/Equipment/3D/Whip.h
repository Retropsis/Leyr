// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Whip.generated.h"

UCLASS()
class LEYR_API AWhip : public AActor
{
	GENERATED_BODY()
	
public:	
	AWhip();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HandleAttackFinished();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Whip;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimationAsset> WhipIdle;
};
