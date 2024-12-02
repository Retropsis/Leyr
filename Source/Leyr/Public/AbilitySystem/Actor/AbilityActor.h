// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/ActorClassInfo.h"
#include "Interaction/AbilityActorInterface.h"
#include "Interaction/CombatInterface.h"
#include "AbilityActor.generated.h"

class UPaperSprite;
class UWidgetComponent;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class LEYR_API AAbilityActor : public AActor, public IAbilitySystemInterface, public IAbilityActorInterface
{
	GENERATED_BODY()
	
public:	
	AAbilityActor();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	FOnASCRegistered OnASCRegistered;
	FOnDeath OnDeath;
	
	//~ AbilityActor Interface
	virtual void DestroyActor_Implementation() override;
	virtual bool IsDestroyed_Implementation() const override { return bDestroyed; }
	virtual int32 GetActorLevel_Implementation() override { return Level; }
	virtual EActorClass GetActorClass_Implementation() override { return ActorClass; }
	//~ AbilityActor Interface

protected:
	virtual void BeginPlay() override;
	virtual void InitializeDefaultAttributes() const;
	virtual void InitAbilityActorInfo();

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	EActorClass ActorClass = EActorClass::Default;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Combat")
	float LifeSpan = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHandleDestruction();

	UPROPERTY(BlueprintReadOnly)
	bool bDestroyed = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Combat")
	USoundBase* DestructionSound;
};
