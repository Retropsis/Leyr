// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "StatusEffectNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UStatusEffectNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
	
public:
	UStatusEffectNiagaraComponent();

	UPROPERTY(VisibleAnywhere)
	FGameplayTag StatusEffectTag;

protected:
	virtual void BeginPlay() override;
	void StatusEffectTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION()
	void OnOwnerDeath(AActor* Owner) { Deactivate(); }
};
