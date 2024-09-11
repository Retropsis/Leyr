// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "PaperCharacter.h"
#include "Interaction/CombatInterface.h"
#include "AbilityActor.generated.h"

class UWidgetComponent;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class LEYR_API AAbilityActor : public AActor, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()
	
public:	
	AAbilityActor();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	//~ Combat Interface
	virtual void Die(const FVector& DeathImpulse) override;
	virtual bool IsDead_Implementation() const override { return  bDead; }
	virtual ECharacterClass GetCharacterClass_Implementation() override { return CharacterClass; }
	virtual int32 GetCharacterLevel_Implementation() override { return Level; }
	virtual FOnASCRegistered& GetOnASCRegistered() override { return OnASCRegistered; }
	virtual FOnDeath& GetOnDeath() override { return OnDeath; }
	FOnASCRegistered OnASCRegistered;
	FOnDeath OnDeath;
	//~ Combat Interface

protected:
	virtual void BeginPlay() override;
	virtual void InitializeDefaultAttributes() const;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Character|Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Character|Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Character|Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Breakable;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Combat")
	float LifeSpan = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Combat")
	USoundBase* DestructionSound;
};
