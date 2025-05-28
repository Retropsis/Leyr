// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "RootMotionGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API URootMotionGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void InitAbility() override;
	virtual void PrepareToEndAbility() override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> Character = nullptr;
	
private:
	void ToggleCapsuleCollision(bool bEnable) const;
};
