// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Core GameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FBaseGameplayTags
{
public:
	static const FBaseGameplayTags& Get() { return GameplayTags;}
	static void InitializeNativeGameplayTags();
 
 /*
  * Vital Attributes
 */
 FGameplayTag Attributes_Vital_Health;
 FGameplayTag Attributes_Vital_Mana;
 
 /*
  * Primary Attributes
  */
FGameplayTag Attributes_Primary_Strength;
FGameplayTag Attributes_Primary_Dexterity;
FGameplayTag Attributes_Primary_Vitality;
FGameplayTag Attributes_Primary_Intelligence;
FGameplayTag Attributes_Primary_Wisdom;
FGameplayTag Attributes_Primary_Spirit;
FGameplayTag Attributes_Primary_Luck;
	
 /*
  * Secondary Attributes
  */
FGameplayTag Attributes_Secondary_PhysicalAttack;
FGameplayTag Attributes_Secondary_MagicalAttack;
FGameplayTag Attributes_Secondary_PhysicalDefense;
FGameplayTag Attributes_Secondary_MagicalDefense;
FGameplayTag Attributes_Secondary_ArmorPenetration;
FGameplayTag Attributes_Secondary_BlockChance;
FGameplayTag Attributes_Secondary_CriticalHitChance;
FGameplayTag Attributes_Secondary_CriticalHitDamage;
FGameplayTag Attributes_Secondary_CriticalHitResistance;
FGameplayTag Attributes_Secondary_HealthRegeneration;
FGameplayTag Attributes_Secondary_ManaRegeneration;
FGameplayTag Attributes_Secondary_MaxHealth;
FGameplayTag Attributes_Secondary_MaxMana;
 
 /*
 * Resistances Attributes
 */
FGameplayTag Attributes_Resistance_None;
FGameplayTag Attributes_Resistance_Physical;
FGameplayTag Attributes_Resistance_Blunt;
FGameplayTag Attributes_Resistance_Cutting;
FGameplayTag Attributes_Resistance_Piercing;
FGameplayTag Attributes_Resistance_Fire;
FGameplayTag Attributes_Resistance_Ice;
FGameplayTag Attributes_Resistance_Wind;
FGameplayTag Attributes_Resistance_Lightning;
FGameplayTag Attributes_Resistance_Holy;
FGameplayTag Attributes_Resistance_Dark;
FGameplayTag Attributes_Resistance_Noxious;
FGameplayTag Attributes_Resistance_Execution;

 /*
  * Meta Attributes
  */
FGameplayTag Attributes_Meta_IncomingXP;
 
 /*
  *  Passive Event Tags
  */
FGameplayTag Event_Update_Equipment_Weapon;
FGameplayTag Event_Update_Equipment_Armor;
  	
 /*
  * Damage Types
  */
FGameplayTag Damage_Physical;
FGameplayTag Damage_Blunt;
FGameplayTag Damage_Cutting;
FGameplayTag Damage_Piercing;
FGameplayTag Damage_Fire;
FGameplayTag Damage_Ice;
FGameplayTag Damage_Wind;
FGameplayTag Damage_Lightning;
FGameplayTag Damage_Holy;
FGameplayTag Damage_Dark;
FGameplayTag Damage_Noxious;
FGameplayTag Damage_Execute;
FGameplayTag Damage_True;

TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

 /*
  * Healing Types
  */
FGameplayTag Healing_Health;
FGameplayTag Healing_Mana;
   	  	
 /*
  * Status Effects
  */
FGameplayTag StatusEffect_Burn;
FGameplayTag StatusEffect_Stun;
FGameplayTag StatusEffect_Poison;
FGameplayTag StatusEffect_Sleep;
FGameplayTag StatusEffect_Silence;
 
FGameplayTag StatusEffect_Chance;
FGameplayTag StatusEffect_Damage;
FGameplayTag StatusEffect_Duration;
FGameplayTag StatusEffect_Frequency;

TMap<FGameplayTag, FGameplayTag> DamageTypesToStatusEffects;

 /*
  * Alteration
  */
FGameplayTag Abilities_Alteration_Berserk_1;
   	  	
 /*
  * Alteration Effects
  */
FGameplayTag AlterationEffect_Alteration_Berserk_1;

TArray<FGameplayTag> AlterationEffects;
 
 /*
  * Abilities
  */
FGameplayTag Abilities_None;
FGameplayTag Abilities_Attack;
FGameplayTag Abilities_Summon;
FGameplayTag Abilities_HitReact;
FGameplayTag Abilities_Fire_FireBolt;
FGameplayTag Abilities_Fire_FireBlast;
FGameplayTag Abilities_Lightning_Electrocute;
FGameplayTag Abilities_Lightning_Bolt_1;
FGameplayTag Abilities_Arcane_ArcaneShards;
FGameplayTag Abilities_Cosmos_Meteor_1;
FGameplayTag Abilities_Noxious_Bio_1;

FGameplayTag Abilities_Cure_1;
FGameplayTag Abilities_Cure_2;

FGameplayTag Abilities_Passive_Barrier;
FGameplayTag Abilities_Passive_HealthDrain;
FGameplayTag Abilities_Passive_ManaDrain;

FGameplayTag Abilities_Status_Locked;
FGameplayTag Abilities_Status_Eligible;
FGameplayTag Abilities_Status_Unlocked;
FGameplayTag Abilities_Status_Equipped;

FGameplayTag Abilities_Type_Offensive;
FGameplayTag Abilities_Type_Passive;
FGameplayTag Abilities_Type_None;

FGameplayTag Abilities_Weapon_Sword_1;
FGameplayTag Abilities_Weapon_Sword_2;
FGameplayTag Abilities_Weapon_Sword_Slash;
FGameplayTag Abilities_Weapon_Fire_FireBolt;
FGameplayTag Abilities_Weapon_Noxious_Bio_1;
FGameplayTag Abilities_Weapon_Aim;
FGameplayTag Abilities_Weapon_Bow_Shot;
 
FGameplayTag Abilities_Combo_Attack_1;
FGameplayTag Abilities_Combo_Attack_2;
FGameplayTag Abilities_Combo_Attack_3;
FGameplayTag Abilities_Combo_Attack_Execute;
 
FGameplayTag Abilities_Combo_Window;
FGameplayTag Abilities_Combo_Window_1;
FGameplayTag Abilities_Combo_Window_2;
FGameplayTag Abilities_Combo_Window_3;
FGameplayTag Abilities_Combo_Window_Execute;
FGameplayTag Abilities_Combo_Window_Begin;
FGameplayTag Abilities_Combo_Window_End;
 
FGameplayTag Abilities_Monk_Basic;
 
FGameplayTag Abilities_Throw_1;
FGameplayTag Abilities_Throw_2;
 
FGameplayTag CombatState_Unoccupied;
FGameplayTag CombatState_Slope;
FGameplayTag CombatState_Rope;
FGameplayTag CombatState_Ladder;
FGameplayTag CombatState_Ledge;
FGameplayTag CombatState_Falling;
FGameplayTag CombatState_Crouching;
FGameplayTag CombatState_UnCrouching;
FGameplayTag CombatState_HitReacting;
FGameplayTag CombatState_Swimming;
FGameplayTag CombatState_Entangled;
 
 FGameplayTagContainer CombatStates;

 /*
  * Gameplay Cues
  */
FGameplayTag GameplayCue_FireBlast;
FGameplayTag GameplayCue_Healing_Cure_1;
FGameplayTag GameplayCue_Healing_Cure_2;
FGameplayTag GameplayCue_Lightning_Bolt_1;
FGameplayTag GameplayCue_Noxious_Bio_1;
FGameplayTag GameplayCue_Cosmos_Meteor_1;
FGameplayTag GameplayCue_Weapon_Sword_Slash;
FGameplayTag GameplayCue_Execute;
 
FGameplayTag GameplayCue_Alteration_Berserk_1;

 /*
  * Abilities Cooldowns
  */
FGameplayTag Cooldown_Throw_1;
FGameplayTag Cooldown_Throw_2;
FGameplayTag Cooldown_Fire_FireBolt;
FGameplayTag Cooldown_Healing_Cure_1;
FGameplayTag Cooldown_Healing_Cure_2;
FGameplayTag Cooldown_Lightning_Bolt_1;
FGameplayTag Cooldown_Noxious_Bio_1;
FGameplayTag Cooldown_Cosmos_Meteor;
FGameplayTag Cooldown_Weapon_Sword_Slash;
FGameplayTag Cooldown_Weapon_Fire_FireBolt;
FGameplayTag Cooldown_Weapon_Bow_Shot;
 
FGameplayTag Cooldown_Alteration_Berserk_1;
 
/*
 * Side Effects
 */
FGameplayTag Effects_HitReact;
 
 /*
  * Combat Sockets
  */
FGameplayTag CombatSocket_Preferred;
FGameplayTag CombatSocket_Weapon;
FGameplayTag CombatSocket_LeftHand;
FGameplayTag CombatSocket_RightHand;
FGameplayTag CombatSocket_Fang;
FGameplayTag CombatSocket_Tail;
FGameplayTag CombatSocket_LeftFeet;
FGameplayTag CombatSocket_RightFeet;

 /*
  * Items
  */
 FGameplayTag Message_Potion_Health_Instant;
 FGameplayTag Message_Potion_Health_Duration;
 FGameplayTag Message_Potion_Mana_Instant;
 FGameplayTag Message_Potion_Mana_Duration;
 
 /*
  * Indicators
  */
 FGameplayTag Indicator_Execute;
 FGameplayTag Indicator_Execute_PercentMissingHealth_Flat;
 FGameplayTag Indicator_Execute_PercentMissingHealth_Scaling;
 FGameplayTag Indicator_Execute_PercentMissingHealth_Stack;
 FGameplayTag Indicator_Entangled;
 FGameplayTag Indicator_Bleeding;
 
 /*
  * Montage / Gameplay Events
  */
FGameplayTag Event_Montage_FireBolt;
FGameplayTag Event_Montage_Bow_Draw;
FGameplayTag Event_Montage_Bow_Shot;
 
 /*
  * Montages
  */
FGameplayTag Montage_Attack_1;
FGameplayTag Montage_Attack_2;
FGameplayTag Montage_Attack_3;
FGameplayTag Montage_Attack_4;
FGameplayTag Montage_Crouch_Attack;
FGameplayTag Montage_Jump_Attack;
FGameplayTag Montage_JumpDown_Attack;
FGameplayTag Montage_Upward_Attack;
FGameplayTag Montage_Rope_Attack;
FGameplayTag Montage_Slope_Attack;
FGameplayTag Montage_Ladder_Attack;
 
FGameplayTag Montage_Window_Combo;
FGameplayTag Montage_Window_Execute;
 
 /*
  * Input Tags
  */
FGameplayTag InputTag_LMB;
FGameplayTag InputTag_RMB;
FGameplayTag InputTag_1;
FGameplayTag InputTag_2;
FGameplayTag InputTag_3;
FGameplayTag InputTag_4;
FGameplayTag InputTag_5;
FGameplayTag InputTag_6;
FGameplayTag InputTag_7;
FGameplayTag InputTag_8;
FGameplayTag InputTag_9;
FGameplayTag InputTag_0;
FGameplayTag InputTag_Passive_1;
FGameplayTag InputTag_Passive_2;
FGameplayTag InputTag_Passive_3;
FGameplayTag InputTag_Passive_4;
FGameplayTag InputTag_Passive_5;
 
FGameplayTag InputTag_Crouch_Attack;
FGameplayTag InputTag_Jump_Attack;
FGameplayTag InputTag_JumpDown_Attack;
FGameplayTag InputTag_Upward_Attack;
 
FGameplayTag InputTag_Throw_1;
FGameplayTag InputTag_Throw_2;
 
 /*
 * Player Tags
 */
FGameplayTag Player_Block_InputPressed;
FGameplayTag Player_Block_InputHeld;
FGameplayTag Player_Block_InputReleased;
FGameplayTag Player_Block_CursorTrace;

private:
	static FBaseGameplayTags GameplayTags;
};
