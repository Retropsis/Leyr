// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperZD/Public/PaperZDAnimationComponent.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class LEYR_API ABaseCharacter : public APaperCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	void ChangeDirections();

protected:
	/*
	 * Ability System
	 */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PaperZD", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPaperZDAnimationComponent> AnimationComponent;
};
