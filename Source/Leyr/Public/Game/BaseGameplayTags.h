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
  * Unique States
  */
 FGameplayTag Player;
 FGameplayTag Player_First;
 FGameplayTag Player_Second;
 FGameplayTag Invincibility;
 FGameplayTag Peaceful;
 FGameplayTag Shackled;
 FGameplayTag Defeated;
 FGameplayTag Poise;
 FGameplayTag Execute;
 
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
FGameplayTag Attributes_Secondary_Poise;
 
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
TMap<FGameplayTag, FGameplayTag> StatusEffectsToResistances;

 /*
  * Healing Types
  */
FGameplayTag Healing_Health;
FGameplayTag Healing_Mana;
   	  	
 /*
  * Status Effects
  */
FGameplayTag StatusEffect_HitReact;
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
TMap<FGameplayTag, FGameplayTag> StatusEffectsToDamageTypes;
 TMap<FGameplayTag, bool> StatusEffectToPeriodicEffectOnApplicationPolicy;

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
FGameplayTag Abilities_Dodge;
FGameplayTag Abilities_Parry;
FGameplayTag Abilities_Roll;
FGameplayTag Abilities_Block;
FGameplayTag Abilities_Shield;
FGameplayTag Abilities_Summon;
FGameplayTag Abilities_HitReact;
FGameplayTag Abilities_Fire_FireBolt;
FGameplayTag Abilities_Fire_FireBlast;
FGameplayTag Abilities_Fire_FireBreath;
FGameplayTag Abilities_Lightning_Electrocute;
FGameplayTag Abilities_Lightning_Bolt_1;
FGameplayTag Abilities_Earth_Earthquake;
FGameplayTag Abilities_Cosmos_Meteor_1;
FGameplayTag Abilities_Noxious_Bio_1;
FGameplayTag Abilities_Noxious_Dart;
FGameplayTag Abilities_Noxious_Orb;
FGameplayTag Abilities_Sorcery;

FGameplayTag Abilities_Throw_1;
FGameplayTag Abilities_Throw_2;
 
FGameplayTag Abilities_ClimbingRope;
FGameplayTag Abilities_SwimImpulse;
FGameplayTag Abilities_RootMotion;
FGameplayTag Abilities_Leap;

FGameplayTag Abilities_Cure_1;
FGameplayTag Abilities_Cure_2;

FGameplayTag Abilities_Passive;
FGameplayTag Abilities_Passive_ListenForEvent;
FGameplayTag Abilities_Passive_Mastery;
FGameplayTag Abilities_Passive_Mastery_Sword;
FGameplayTag Abilities_Passive_Mastery_Dagger;
FGameplayTag Abilities_Passive_Mastery_Axe;
FGameplayTag Abilities_Passive_Mastery_Blunt;
FGameplayTag Abilities_Passive_Mastery_DualWield;
FGameplayTag Abilities_Passive_Mastery_GreatSword;
FGameplayTag Abilities_Passive_Mastery_Bow;
FGameplayTag Abilities_Passive_Mastery_Crossbow;
FGameplayTag Abilities_Passive_Mastery_Buckler;
FGameplayTag Abilities_Passive_Mastery_Sorcery;
FGameplayTag Abilities_Passive_Mastery_Throw;
 
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
FGameplayTag Abilities_Type_ActionSlot;
FGameplayTag Abilities_Type_ActionSlot_Main;
FGameplayTag Abilities_Type_ActionSlot_Sub;
FGameplayTag Abilities_Type_PassiveSlot;

 /*
  * Player Abilities
  */
FGameplayTag Abilities_Weapon_Sword_1;
FGameplayTag Abilities_Weapon_Sword_2;
FGameplayTag Abilities_Weapon_Sword_Slash;
FGameplayTag Abilities_Weapon_Fire_FireBolt;
FGameplayTag Abilities_Weapon_Noxious_Bio_1;
FGameplayTag Abilities_Weapon_Aim;
FGameplayTag Abilities_Weapon_Bow_Shot;
FGameplayTag Abilities_Weapon_Throw_Spear;
FGameplayTag Abilities_Weapon_Throw_Kunai;
FGameplayTag Abilities_Weapon_Throw_Boomerang;
 FGameplayTag Abilities_Weapon_Throw_Knife;
 
FGameplayTag Abilities_Weapon;
FGameplayTag Abilities_Weapon_Monk;
FGameplayTag Abilities_Weapon_Monk_States;
FGameplayTag Abilities_Weapon_Monk_Combo;
FGameplayTag Abilities_Weapon_Monk_Extra;
FGameplayTag Abilities_Weapon_Monk_Execute;
FGameplayTag Abilities_Weapon_OneHanded_Attack;
FGameplayTag Abilities_Weapon_OneHanded_States;
FGameplayTag Abilities_Weapon_OneHanded_Combo;
FGameplayTag Abilities_Weapon_OneHanded_Extra;
FGameplayTag Abilities_Weapon_OneHanded_Execute;
FGameplayTag Abilities_Weapon_TwoHanded_Attack;
FGameplayTag Abilities_Weapon_TwoHanded_States;
FGameplayTag Abilities_Weapon_TwoHanded_Combo;
FGameplayTag Abilities_Weapon_TwoHanded_Extra;
FGameplayTag Abilities_Weapon_TwoHanded_Execute;
FGameplayTag Abilities_Weapon_Whip_Attack;
 
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
FGameplayTag Abilities_Combo_Window_Cue;
FGameplayTag Abilities_Combo_Window_End;

 /*
  * AI Abilities
  */
FGameplayTag Abilities_JumpToLocation;
FGameplayTag Abilities_Range_GroundBurst;

 /*
  * Combat States
  */
FGameplayTag CombatState_Unoccupied;
FGameplayTag CombatState_Defeated;
FGameplayTag CombatState_Peaceful;
FGameplayTag CombatState_Elevator;
FGameplayTag CombatState_UnCrouching;
FGameplayTag CombatState_Condition_Walking;
FGameplayTag CombatState_Condition_Slope;
FGameplayTag CombatState_Condition_Rope;
FGameplayTag CombatState_Condition_Hook;
FGameplayTag CombatState_Condition_Ladder;
FGameplayTag CombatState_Condition_Ledge;
FGameplayTag CombatState_Condition_Climbing;
FGameplayTag CombatState_Condition_Falling;
FGameplayTag CombatState_Condition_Swimming;
FGameplayTag CombatState_Condition_Crouching;
FGameplayTag CombatState_Condition_Entangled;
FGameplayTag CombatState_Transient_HitReacting;
FGameplayTag CombatState_Transient_Aiming;
FGameplayTag CombatState_Transient_Rope;
FGameplayTag CombatState_Transient_Ledge;
FGameplayTag CombatState_Transient_Dodging;
FGameplayTag CombatState_Transient_Rolling;
 
FGameplayTag CombatState_Execute;
 
FGameplayTag CombatState_Directional_Upward;
FGameplayTag CombatState_Directional_Downward;
FGameplayTag CombatState_Directional_Forward;
FGameplayTag CombatState_Directional_ForwardUp;
FGameplayTag CombatState_Directional_ForwardDown;
	
FGameplayTag CombatState_Rule;
FGameplayTag CombatState_Rule_Block;
FGameplayTag CombatState_Rule_Block_All;
FGameplayTag CombatState_Rule_Block_Movement;
FGameplayTag CombatState_Rule_Block_Ability;
FGameplayTag CombatState_Rule_Stop;
FGameplayTag CombatState_Rule_Stop_All;
FGameplayTag CombatState_Rule_Stop_Movement;
FGameplayTag CombatState_Rule_Stop_Ability;
 
 FGameplayTagContainer CombatStates;
 FGameplayTagContainer CombatDirections;
 FGameplayTagContainer ToPreviousStateFilter;
 FGameplayTagContainer ToUnoccupiedStateFilter;
 FGameplayTagContainer CombatStateRules;

 /*
  * Gameplay Cues
  */
FGameplayTag GameplayCue_FireBlast;
FGameplayTag GameplayCue_Electrocute;
FGameplayTag GameplayCue_Electrocute_Loop;
FGameplayTag GameplayCue_Earth_Earthquake;
FGameplayTag GameplayCue_Healing_Cure_1;
FGameplayTag GameplayCue_Healing_Cure_2;
FGameplayTag GameplayCue_Lightning_Bolt_1;
FGameplayTag GameplayCue_Noxious_Bio_1;
FGameplayTag GameplayCue_Cosmos_Meteor_1;
FGameplayTag GameplayCue_Weapon;
FGameplayTag GameplayCue_Weapon_Monk;
FGameplayTag GameplayCue_Weapon_Sword_Slash;
FGameplayTag GameplayCue_Weapon_Bow_Shot;
FGameplayTag GameplayCue_Bite;
FGameplayTag GameplayCue_Execute;
FGameplayTag GameplayCue_Weapon_Throw;
FGameplayTag GameplayCue_Shield;
FGameplayTag GameplayCue_WaterSplash_Small;
FGameplayTag GameplayCue_WaterSplash_Regular;
FGameplayTag GameplayCue_WaterSplash_Large;
 
FGameplayTag GameplayCue_Alteration_Berserk_1;
 
 /*
 * Asset Cues
 */
 FGameplayTag AssetCue_ImpactSound;
 FGameplayTag AssetCue_ImpactEffect;

 /*
  * Abilities Cooldowns
  */
FGameplayTag Cooldown_Throw_1;
FGameplayTag Cooldown_Throw_2;
FGameplayTag Cooldown_Fire_FireBolt;
FGameplayTag Cooldown_Fire_FireBlast;
FGameplayTag Cooldown_Lightning_Electrocute;
FGameplayTag Cooldown_Earth_Earthquake;
FGameplayTag Cooldown_Healing_Cure_1;
FGameplayTag Cooldown_Healing_Cure_2;
FGameplayTag Cooldown_Lightning_Bolt_1;
FGameplayTag Cooldown_Noxious_Bio_1;
FGameplayTag Cooldown_Cosmos_Meteor;
FGameplayTag Cooldown_Weapon_Sword_Slash;
FGameplayTag Cooldown_Weapon_Fire_FireBolt;
FGameplayTag Cooldown_Weapon_Bow_Shot;
 
FGameplayTag Cooldown_Alteration_Berserk_1;
 
FGameplayTag Cooldown_Item_Instant;
FGameplayTag Cooldown_Item_Short;
FGameplayTag Cooldown_Item_Average;
FGameplayTag Cooldown_Item_Long;
FGameplayTag Cooldown_Item_VeryLong;
 
FGameplayTag Cooldown_SwimImpulse;
 
 /*
  * Inventory Cost Tags
  */
 FGameplayTag Cost;
 FGameplayTag Cost_None;
 FGameplayTag Cost_Arrow;
 FGameplayTag Cost_Bolt;
 FGameplayTag Cost_Bone;
 FGameplayTag Cost_Feather;
 
 /*
  * Combat Sockets
  */
FGameplayTag CombatSocket_Preferred;
FGameplayTag CombatSocket_Weapon;
FGameplayTag CombatSocket_Weapon_UpperBody;
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
FGameplayTag Event_Montage_Electrocute;
FGameplayTag Event_Montage_Bow_Draw;
FGameplayTag Event_Montage_Bow_Shot;
 
 /*
  * Montages
  */
FGameplayTag Montage_Attack_1;
FGameplayTag Montage_Attack_2;
FGameplayTag Montage_Attack_3;
FGameplayTag Montage_Attack_4;
FGameplayTag Montage_Casting;
FGameplayTag Montage_Shielding;
FGameplayTag Montage_Shielding_Begin;
FGameplayTag Montage_Shielding_End;
FGameplayTag Montage_Throwing;
FGameplayTag Montage_Using;
FGameplayTag Montage_Crouch_Attack;
FGameplayTag Montage_Jump_Attack;
FGameplayTag Montage_JumpForwardUp_Attack;
FGameplayTag Montage_JumpForwardDown_Attack;
FGameplayTag Montage_JumpUpward_Attack;
FGameplayTag Montage_ForwardUp_Attack;
FGameplayTag Montage_Falling_Attack;
FGameplayTag Montage_Ascent_Attack;
FGameplayTag Montage_Upward_Attack;
FGameplayTag Montage_Rope_Attack;
FGameplayTag Montage_Slope_Attack;
FGameplayTag Montage_Ladder_Attack;
FGameplayTag Montage_Roll_End;
FGameplayTag Montage_Roll_Attack;
FGameplayTag Montage_Swim_Attack;
FGameplayTag Montage_Dodge_End;
FGameplayTag Montage_EndAbility;
FGameplayTag Montage_Parry;
FGameplayTag Montage_Parry_Begin;
FGameplayTag Montage_Parry_End;
FGameplayTag Montage_Execute;
FGameplayTag Montage_HitStop;
FGameplayTag Montage_Impulse;
 
FGameplayTag Montage_Window_Combo;
FGameplayTag Montage_Window_Execute;
 
FGameplayTag Montage_Cue;
FGameplayTag Montage_Cue_1;
FGameplayTag Montage_Cue_2;
FGameplayTag Montage_Cue_3;
FGameplayTag Montage_Cue_4;
 
 /*
  * Equipment Tags
  */
 FGameplayTag Equipment;
 FGameplayTag Equipment_ActionSlot;
 FGameplayTag Equipment_ActionSlot_0;
 FGameplayTag Equipment_ActionSlot_1;
 FGameplayTag Equipment_ActionSlot_2;
 FGameplayTag Equipment_ActionSlot_3;
 FGameplayTag Equipment_ActionSlot_MainHand;
 FGameplayTag Equipment_ActionSlot_OffHand;
 FGameplayTag Equipment_ActionSlot_Range;
 FGameplayTag Equipment_Headgear;
 FGameplayTag Equipment_Neck;
 FGameplayTag Equipment_UpperBody;
 FGameplayTag Equipment_LowerBody;
 FGameplayTag Equipment_Footwear;
 FGameplayTag Equipment_Wrist;
 FGameplayTag Equipment_Waist;
 FGameplayTag Equipment_Ammunition;
 FGameplayTag Equipment_Ammunition_1;
 FGameplayTag Equipment_Ammunition_2;
 FGameplayTag Equipment_Ammunition_3;

 TMap<FGameplayTag, FGameplayTag> EquipmentSlotToInputTags;
 TMap<FGameplayTag, FGameplayTag> AbilitySlotToInputTags;
 
 /*
  * Input Tags
  */
FGameplayTag InputTag;
FGameplayTag InputTag_None;
FGameplayTag InputTag_MainHand;
FGameplayTag InputTag_OffHand;
FGameplayTag InputTag_Shoulder_Left;
FGameplayTag InputTag_Shoulder_Right;
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
FGameplayTag InputTag_Throw_1;
FGameplayTag InputTag_Throw_2;
FGameplayTag InputTag_Execute_LMB;
FGameplayTag InputTag_Execute_RMB;
FGameplayTag InputTag_Execute_1;
 
 /*
 * Player Tags
 */
FGameplayTag Player_Block_InputPressed;
FGameplayTag Player_Block_InputHeld;
FGameplayTag Player_Block_InputReleased;
FGameplayTag Player_Block_CursorTrace;

 /*
 * AI Pattern Tags
 */
 FGameplayTag AI_Pattern;
 FGameplayTag AI_Pattern_None;
 FGameplayTag AI_Pattern_Random;
 FGameplayTag AI_Pattern_Phase1;
 FGameplayTag AI_Pattern_Phase1_Random;
 FGameplayTag AI_Pattern_Phase1_A;
 FGameplayTag AI_Pattern_Phase1_B;
 FGameplayTag AI_Pattern_Phase1_C;
 FGameplayTag AI_Pattern_Phase1_D;
 FGameplayTag AI_Pattern_Phase2;
 FGameplayTag AI_Pattern_Phase2_Random;
 FGameplayTag AI_Pattern_Phase2_A;
 FGameplayTag AI_Pattern_Phase2_B;
 FGameplayTag AI_Pattern_Phase2_C;
 FGameplayTag AI_Pattern_Phase2_D;
 FGameplayTag AI_Pattern_Phase3;
 FGameplayTag AI_Pattern_Phase3_Random;
 FGameplayTag AI_Pattern_Phase3_A;
 FGameplayTag AI_Pattern_Phase3_B;
 FGameplayTag AI_Pattern_Phase3_C;
 FGameplayTag AI_Pattern_Phase3_D;

/*
 * Spawn Positions
*/
FGameplayTag Spawn_Left;
FGameplayTag Spawn_Right;
FGameplayTag Spawn_Above;
FGameplayTag Spawn_Below;

/*
 * Visual Effects
 */
FGameplayTag VisualEffect_HitReactFlash;
 
private:
	static FBaseGameplayTags GameplayTags;
};
