// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

// typedef is specific to the FGameplayAttribute() signature, but TStaticFunPtr is generic to any signature chosen
//typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class LEYR_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBaseAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
		
	/*
	 * Primary Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Strength, Category="Attributes|Primary")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Strength);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Dexterity, Category="Attributes|Primary")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Dexterity);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Vitality, Category="Attributes|Primary")
	FGameplayAttributeData Vitality;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Vitality);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Intelligence, Category="Attributes|Primary")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Intelligence);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Wisdom, Category="Attributes|Primary")
	FGameplayAttributeData Wisdom;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Wisdom);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Spirit, Category="Attributes|Primary")
	FGameplayAttributeData Spirit;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Spirit);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Luck, Category="Attributes|Primary")
	FGameplayAttributeData Luck;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Luck);

	/*
	 * Secondary Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PhysicalAttack, Category="Attributes|Secondary")
	FGameplayAttributeData PhysicalAttack;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, PhysicalAttack);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MagicalAttack, Category="Attributes|Secondary")
	FGameplayAttributeData MagicalAttack;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MagicalAttack);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalDefense, Category = "Attributes|Secondary")
	FGameplayAttributeData PhysicalDefense;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, PhysicalDefense);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicalDefense, Category = "Attributes|Secondary")
	FGameplayAttributeData MagicalDefense;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet,  MagicalDefense);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Attributes|Secondary")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Attributes|Secondary")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Attributes|Secondary")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Attributes|Secondary")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Attributes|Secondary")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Attributes|Secondary")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Attributes|Secondary")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ManaRegeneration);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Attributes|Secondary")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Attributes|Secondary")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxMana);
	
	/*
	 * Resistance Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PhysicalResistance, Category="Attributes|Resistances")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, PhysicalResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BluntResistance, Category="Attributes|Resistances")
	FGameplayAttributeData BluntResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BluntResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CuttingResistance, Category="Attributes|Resistances")
	FGameplayAttributeData CuttingResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CuttingResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PiercingResistance, Category="Attributes|Resistances")
	FGameplayAttributeData PiercingResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, PiercingResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_FireResistance, Category="Attributes|Resistances")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, FireResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_IceResistance, Category="Attributes|Resistances")
	FGameplayAttributeData IceResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, IceResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_WindResistance, Category="Attributes|Resistances")
	FGameplayAttributeData WindResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, WindResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_LightningResistance, Category="Attributes|Resistances")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, LightningResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_HolyResistance, Category="Attributes|Resistances")
	FGameplayAttributeData HolyResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, HolyResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_DarkResistance, Category="Attributes|Resistances")
	FGameplayAttributeData DarkResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, DarkResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_NoxiousResistance, Category="Attributes|Resistances")
	FGameplayAttributeData NoxiousResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, NoxiousResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ExecutionResistance, Category="Attributes|Resistances")
	FGameplayAttributeData ExecutionResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ExecutionResistance);

	/*
	 * Vital Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Attributes|Vital")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Attributes|Vital")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Mana);

	/*
	 * Meta Attributes
	 */
	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, IncomingDamage);
	
	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData IncomingHealing;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, IncomingHealing);
	
	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, IncomingXP);

	/*
	 * Rep Notifies
	*/
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	
	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;
	
	UFUNCTION()
	void OnRep_Vitality(const FGameplayAttributeData& OldVitality) const;
	
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	
	UFUNCTION()
	void OnRep_Wisdom(const FGameplayAttributeData& OldWisdom) const;
	
	UFUNCTION()
	void OnRep_Spirit(const FGameplayAttributeData& OldSpirit) const;

	UFUNCTION()
	void OnRep_Luck(const FGameplayAttributeData& OldLuck) const;

	/* Secondary Attributes Rep Notifies */
	UFUNCTION()
	void OnRep_PhysicalAttack(const FGameplayAttributeData& OldPhysicalAttack) const;
	
	UFUNCTION()
	void OnRep_MagicalAttack(const FGameplayAttributeData& OldMagicalAttack) const;
	
	UFUNCTION()
	void OnRep_PhysicalDefense(const FGameplayAttributeData& OldPhysicalDefense) const;

	UFUNCTION()
	void OnRep_MagicalDefense(const FGameplayAttributeData& OldMagicalDefense) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	
	/* Resistance Attributes Rep Notifies */
	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;
	
	UFUNCTION()
	void OnRep_BluntResistance(const FGameplayAttributeData& OldBluntResistance) const;
	
	UFUNCTION()
	void OnRep_CuttingResistance(const FGameplayAttributeData& OldCuttingResistance) const;
	
	UFUNCTION()
	void OnRep_PiercingResistance(const FGameplayAttributeData& OldPiercingResistance) const;
	
	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;
	
	UFUNCTION()
	void OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const;
	
	UFUNCTION()
	void OnRep_WindResistance(const FGameplayAttributeData& OldWindResistance) const;
	
	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const;
	
	UFUNCTION()
	void OnRep_HolyResistance(const FGameplayAttributeData& OldHolyResistance) const;
	
	UFUNCTION()
	void OnRep_DarkResistance(const FGameplayAttributeData& OldDarkResistance) const;
	
	UFUNCTION()
	void OnRep_NoxiousResistance(const FGameplayAttributeData& OldNoxiousResistance) const;
	
	UFUNCTION()
	void OnRep_ExecutionResistance(const FGameplayAttributeData& OldExecutionResistance) const;
	
	/* Vital Attributes */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const;
	void SendXPEvent(const FEffectProperties& Props);
	
	void HandleIncomingDamage(const FEffectProperties& Props);
	void HandleIncomingXP(const FEffectProperties& Props);
	void HandleStatusEffect(const FEffectProperties& Props);
	
	bool bRefillHealth = false;
	bool bRefillMana = false;
};
