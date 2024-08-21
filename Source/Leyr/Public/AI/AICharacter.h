// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AI/BaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Controller/OverlayWidgetController.h"
#include "AICharacter.generated.h"

class ABaseAIController;
class UBehaviorTree;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class LEYR_API AAICharacter : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAICharacter();
	virtual void PossessedBy(AController* NewController) override;
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	/** Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() override { return Level; }
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override { CombatTarget = InCombatTarget; }
	virtual AActor* GetCombatTarget_Implementation() const override { return CombatTarget; }
	virtual void Die() override;
	/** end Combat Interface */
	
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadOnly, Category="Character|Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Combat")
	float BaseWalkSpeed = 60.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Combat")
	float LifeSpan = 5.f;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	UPROPERTY(EditAnywhere, Category = "Character|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ABaseAIController> BaseAIController;
};
