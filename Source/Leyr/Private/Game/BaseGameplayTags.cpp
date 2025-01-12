// @ Retropsis 2024-2025.

#include "Game/BaseGameplayTags.h"
#include "GameplayTagsManager.h"

FBaseGameplayTags FBaseGameplayTags::GameplayTags;

void FBaseGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Vital Attributes
	*/
	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"), FString("Attributes Vital Health"));
	GameplayTags.Attributes_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Mana"), FString("Attributes Vital Mana"));
	
	/*
	 * Primary Attributes
	 */
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"), FString("Primary factor for physical attacks"));
	GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Dexterity"), FString("Primary factor for accuracy"));
	GameplayTags.Attributes_Primary_Vitality = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vitality"), FString("Primary factor for armor and health"));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), FString("Primary factor for magical attacks"));
	GameplayTags.Attributes_Primary_Wisdom = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Wisdom"), FString("Primary factor for magical knowledge"));
	GameplayTags.Attributes_Primary_Spirit = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Spirit"), FString("Primary factor for mana and magical resistance"));
	GameplayTags.Attributes_Primary_Luck = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Luck"), FString("Random luck, can't be attributed"));
	
	/*
	 * Secondary Attributes
	 */
	GameplayTags.Attributes_Secondary_PhysicalAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.PhysicalAttack"), FString("Reduces damage taken, improves PhysicalAttack"));
	GameplayTags.Attributes_Secondary_MagicalAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MagicalAttack"), FString("Reduces damage taken, improves MagicalAttack"));
	GameplayTags.Attributes_Secondary_PhysicalDefense = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.PhysicalDefense"), FString("Reduces damage taken, improves Armor"));
	GameplayTags.Attributes_Secondary_MagicalDefense = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MagicalDefense"), FString("Reduces damage taken, improves Armor"));
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"), FString("Ignores percentage of enemy's armor, chance for critical hit"));
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"), FString("Chance to reduce receiving damage"));
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"), FString("Chance for a critical hit"));
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"), FString("Critical hit damage amount"));
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"), FString("Reduces damage taken from critical hits"));
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"), FString("Amount of health healed every 1 second"));
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"), FString("Amount of mana healed every 1 second"));
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString("Max amount of health"));
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"), FString("Max amount of mana"));
	
	/*
	 * Resistance Attributes
	 */
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"), FString("Resistance to Physical Damage"));
	GameplayTags.Attributes_Resistance_Blunt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Blunt"), FString("Resistance to Blunt Damage"));
	GameplayTags.Attributes_Resistance_Cutting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Cutting"), FString("Resistance to Cutting Damage"));
	GameplayTags.Attributes_Resistance_Piercing = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Piercing"), FString("Resistance to Piercing Damage"));
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString("Resistance to Fire Damage"));
	GameplayTags.Attributes_Resistance_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Ice"), FString("Resistance to Ice Damage"));
	GameplayTags.Attributes_Resistance_Wind = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Wind"), FString("Resistance to Wind Damage"));
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"), FString("Resistance to Lightning Damage"));
	GameplayTags.Attributes_Resistance_Holy= UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Holy"), FString("Resistance to Holy Damage"));
	GameplayTags.Attributes_Resistance_Dark = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Dark"), FString("Resistance to Dark Damage"));
	GameplayTags.Attributes_Resistance_Noxious = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Noxious"), FString("Resistance to Noxious Damage"));
	GameplayTags.Attributes_Resistance_Execution = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Execution"), FString("Resistance to Execution Damage"));
	
	/*
	 * Meta Attributes
	 */
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Meta.IncomingXP"), FString("Meta Attribute for Incoming XP"));
	
	/*
	 *  Passive Event Tags
	 */
	GameplayTags.Event_Update_Equipment_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Update.Equipment.Weapon"), FString("Event Update Equipment Weapon"));
	GameplayTags.Event_Update_Equipment_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Update.Equipment.Armor"), FString("Event Update Equipment Armor"));
	
	/*
	 * Damage Types
	 */
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString("Physical Damage"));
	GameplayTags.Damage_Blunt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Blunt"), FString("Blunt Damage"));
	GameplayTags.Damage_Cutting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Cutting"), FString("Cutting Damage"));
	GameplayTags.Damage_Piercing = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Piercing"), FString("Piercing Damage"));
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), FString("Fire Damage"));
	GameplayTags.Damage_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Ice"), FString("Ice Damage"));
	GameplayTags.Damage_Wind = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Wind"), FString("Wind Damage"));
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"), FString("Lightning Damage"));
	GameplayTags.Damage_Holy = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Holy"), FString("Holy Damage"));
	GameplayTags.Damage_Dark = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Dark"), FString("Dark Damage"));
	GameplayTags.Damage_Noxious = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Noxious"), FString("Noxious Damage"));
	GameplayTags.Damage_Execute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Execute"), FString("Execute Damage"));
	GameplayTags.Damage_True = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.True"), FString("True Damage"));

	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Blunt, GameplayTags.Attributes_Resistance_Blunt);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Cutting, GameplayTags.Attributes_Resistance_Cutting);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Piercing, GameplayTags.Attributes_Resistance_Piercing);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Ice, GameplayTags.Attributes_Resistance_Ice);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Wind, GameplayTags.Attributes_Resistance_Wind);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Holy, GameplayTags.Attributes_Resistance_Holy);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Dark, GameplayTags.Attributes_Resistance_Dark);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Noxious, GameplayTags.Attributes_Resistance_Noxious);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Execute, GameplayTags.Attributes_Resistance_Execution);
   	  	
	/*
	 * Healing Types
	 */
	GameplayTags.Healing_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Healing.Health"), FString("Healing Health"));
	GameplayTags.Healing_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Healing.Mana"), FString("Healing Mana"));
	
	/*
	 * Status Effects
	 */
	GameplayTags.StatusEffect_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Burn"), FString("Status Effect from Fire"));
	GameplayTags.StatusEffect_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Stun"), FString("Status Effect from Ice"));
	GameplayTags.StatusEffect_Sleep = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Sleep"), FString("Status Effect"));
	GameplayTags.StatusEffect_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Poison"), FString("Status Effect"));
	GameplayTags.StatusEffect_Silence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Silence"), FString("Status Effect"));

	GameplayTags.StatusEffect_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Chance"), FString("Status Effect Chance"));
	GameplayTags.StatusEffect_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Duration"), FString("Status Effect Duration"));
	GameplayTags.StatusEffect_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Damage"), FString("Status Effect Damage"));
	GameplayTags.StatusEffect_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Frequency"), FString("Status Effect Frequency"));
	
	// TODO: Should work differently
	GameplayTags.DamageTypesToStatusEffects.Add(GameplayTags.Damage_Fire, GameplayTags.StatusEffect_Burn);
	GameplayTags.DamageTypesToStatusEffects.Add(GameplayTags.Damage_Lightning, GameplayTags.StatusEffect_Stun);
	GameplayTags.DamageTypesToStatusEffects.Add(GameplayTags.Damage_Ice, GameplayTags.StatusEffect_Silence);
	GameplayTags.DamageTypesToStatusEffects.Add(GameplayTags.Damage_Wind, GameplayTags.StatusEffect_Sleep);
	GameplayTags.DamageTypesToStatusEffects.Add(GameplayTags.Damage_Noxious, GameplayTags.StatusEffect_Poison);
		
	/*
	 * Alteration
	 */
	GameplayTags.Abilities_Alteration_Berserk_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Alteration.Berserk.1"), FString("Abilities Alteration Berserk 1"));
	
	/*
	 * Alteration Effects
	 */
	GameplayTags.AlterationEffect_Alteration_Berserk_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AlterationEffect.Alteration.Berserk.1"), FString("AlterationEffect Alteration Berserk 1"));
	GameplayTags.AlterationEffects.Add(GameplayTags.AlterationEffect_Alteration_Berserk_1);
	
	/*
	 * Abilities
	 */
	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.None"), FString("None"));
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"), FString("Melee Attack"));
	GameplayTags.Abilities_Parry = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Parry"), FString("Melee Parry"));
	GameplayTags.Abilities_Block = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Block"), FString("Melee Block"));
	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Summon"), FString("Ability Summon"));
	
	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBolt"), FString("Fire Element Ability FireBolt"));
	GameplayTags.Abilities_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBlast"), FString("Fire Element Ability FireBlast"));
	GameplayTags.Abilities_Fire_FlameThrower = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FlameThrower"), FString("Fire Element Ability FlameThrower"));
	GameplayTags.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lightning.Electrocute"), FString("Lightning Element Electrocute Ability"));
	GameplayTags.Abilities_Lightning_Bolt_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lightning.Bolt.1"), FString("Abilities Lightning Bolt 1"));
	GameplayTags.Abilities_Noxious_Bio_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Noxious.Bio.1"), FString("Abilities Noxious Bio 1"));
	GameplayTags.Abilities_Cosmos_Meteor_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Cosmos.Meteor.1"), FString("Abilities Cosmos Meteor 1"));
	GameplayTags.Abilities_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Arcane.ArcaneShards"), FString("Arcane Element Shards Ability"));

	GameplayTags.Abilities_Cure_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Cure.1"), FString("Abilities Cure 1"));
	GameplayTags.Abilities_Cure_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Cure.2"), FString("Abilities Cure 2"));
	
	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.HitReact"), FString("HitReact Ability Effect"));
	
	GameplayTags.Abilities_Passive_Barrier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.Barrier"), FString("Passive Ability Barrier"));
	GameplayTags.Abilities_Passive_HealthDrain = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.HealthDrain"), FString("Passive Ability Health Drain"));
	GameplayTags.Abilities_Passive_ManaDrain = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.ManaDrain"), FString("Passive Ability Mana Drain"));
	
	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Locked"), FString("Abilities Status Locked"));
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Eligible"), FString("Abilities Status Eligible"));
	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Unlocked"), FString("Abilities Status Unlocked"));
	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Equipped"), FString("Abilities Status Equipped"));
	
	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.None"), FString("Abilities Type None"));
	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Offensive"), FString("Abilities Type Offensive"));
	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Passive"), FString("Abilities Type Passive"));

	/*
	 * Melee Weapons
	 */
	GameplayTags.Abilities_Weapon_Sword_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Sword.1"), FString("Abilities Weapon Sword 1"));
	GameplayTags.Abilities_Weapon_Sword_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Sword.2"), FString("Abilities Weapon Sword 2"));
	GameplayTags.Abilities_Weapon_Sword_Slash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Sword.Slash"), FString("Abilities Weapon Sword Slash"));
	
	GameplayTags.Abilities_Weapon_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Fire.FireBolt"), FString("Abilities Fire FireBolt Weapon Version"));
	GameplayTags.Abilities_Weapon_Noxious_Bio_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Noxious.Bio.1"), FString("Abilities Weapon Noxious Bio 1"));
	
	GameplayTags.Abilities_Weapon_OneHanded_States = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.OneHanded.States"), FString("Abilities Weapon OneHanded States"));
	GameplayTags.Abilities_Weapon_OneHanded_Combo = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.OneHanded.Combo"), FString("Abilities Weapon OneHanded Combo"));
	GameplayTags.Abilities_Weapon_OneHanded_Extra = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.OneHanded.Extra"), FString("Abilities Weapon OneHanded Extra"));
	GameplayTags.Abilities_Weapon_TwoHanded_States = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.TwoHanded.States"), FString("Abilities Weapon TwoHanded States"));
	GameplayTags.Abilities_Weapon_TwoHanded_Combo = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.TwoHanded.Combo"), FString("Abilities Weapon TwoHanded Combo"));
	GameplayTags.Abilities_Weapon_TwoHanded_Extra = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.TwoHanded.Extra"), FString("Abilities Weapon TwoHanded Extra"));

	/*
	 * Range Weapons
	 */
	GameplayTags.Abilities_Weapon_Aim = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Aim"), FString("Abilities Weapon Aim"));
	GameplayTags.Abilities_Weapon_Bow_Shot = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Bow.Shot"), FString("Abilities Weapon Bow Shot"));
	GameplayTags.Abilities_Weapon_Spear_Throw = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Spear.Throw"), FString("Abilities Weapon Spear Throw"));

	/*
	 * Fighting
	 */
	GameplayTags.Abilities_Weapon_Monk_States = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Monk.States"), FString("Abilities Weapon Monk States"));
	GameplayTags.Abilities_Weapon_Monk_Combo = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Monk.Combo"), FString("Abilities Weapon Monk Combo"));
	GameplayTags.Abilities_Weapon_Monk_Extra = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Monk.Extra"), FString("Abilities Weapon Monk Extra"));
	
	GameplayTags.Abilities_Throw_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Throw.1"), FString("Abilities Throw 1"));
	GameplayTags.Abilities_Throw_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Throw.2"), FString("Abilities Throw 2"));

	/*
	 * Combo
	 */
	GameplayTags.Abilities_Combo_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Combo.Attack.1"), FString("Abilities Combo Attack 1"));
	GameplayTags.Abilities_Combo_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Combo.Attack.2"), FString("Abilities Combo Attack 2"));
	GameplayTags.Abilities_Combo_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Combo.Attack.3"), FString("Abilities Combo Attack 3"));
	GameplayTags.Abilities_Combo_Attack_Execute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Combo.Attack.Execute"), FString("Abilities Combo Attack Execute"));

	/*
	 * Ability Window
	 */
	GameplayTags.Abilities_Combo_Window = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Combo.Window"), FString("Abilities Combo Window"));
	GameplayTags.Abilities_Combo_Window_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Combo.Window.1"), FString("Abilities Combo Window 1"));
	GameplayTags.Abilities_Combo_Window_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Combo.Window.2"), FString("Abilities Combo Window 2"));
	GameplayTags.Abilities_Combo_Window_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Combo.Window.3"), FString("Abilities Combo Window 3"));
	GameplayTags.Abilities_Combo_Window_Begin = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Combo.Window.Begin"), FString("Abilities Combo Window Begin"));
	GameplayTags.Abilities_Combo_Window_End = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Combo.Window.End"), FString("Abilities Combo Window End"));
	GameplayTags.Abilities_Combo_Window_Execute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Combo.Window.Execute"), FString("Abilities Combo Window Execute"));

	/*
	 * Combat States
	 */
	GameplayTags.CombatState_Unoccupied = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Unoccupied"), FString("CombatState Unoccupied"));
	GameplayTags.CombatState_Walking = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Walking"), FString("CombatState Walking"));
	GameplayTags.CombatState_WalkingPeaceful = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.WalkingPeaceful"), FString("CombatState WalkingPeaceful"));
	GameplayTags.CombatState_Aiming = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Aiming"), FString("CombatState Aiming"));
	GameplayTags.CombatState_Slope = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Slope"), FString("CombatState Slope"));
	GameplayTags.CombatState_Ledge = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Ledge"), FString("CombatState Ledge"));
	GameplayTags.CombatState_Rope = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Rope"), FString("CombatState Rope"));
	GameplayTags.CombatState_Hook = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Hook"), FString("CombatState Hook"));
	GameplayTags.CombatState_Ladder = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Ladder"), FString("CombatState Ladder"));
	GameplayTags.CombatState_Elevator = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Elevator"), FString("CombatState Elevator"));
	GameplayTags.CombatState_Climbing = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Climbing"), FString("CombatState Climbing"));
	GameplayTags.CombatState_Hopping = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Hopping"), FString("CombatState Hopping"));
	GameplayTags.CombatState_Falling = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Falling"), FString("CombatState Falling"));
	GameplayTags.CombatState_Crouching = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Crouching"), FString("CombatState Crouching"));
	GameplayTags.CombatState_UnCrouching = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.UnCrouching"), FString("CombatState UnCrouching"));
	GameplayTags.CombatState_HitReacting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.HitReacting"), FString("CombatState HitReacting"));
	GameplayTags.CombatState_Swimming = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Swimming"), FString("CombatState Swimming"));
	GameplayTags.CombatState_Entangled = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Entangled"), FString("CombatState Entangled"));
	GameplayTags.CombatState_Dodging = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Dodging"), FString("CombatState Dodging"));
	GameplayTags.CombatState_Rolling = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Rolling"), FString("CombatState Rolling"));
	
	GameplayTags.CombatState_Directional_Upward = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Directional.Upward"), FString("CombatState Directional Upward"));
	GameplayTags.CombatState_Directional_Downward = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Directional.Downward"), FString("CombatState Directional Downward"));
	GameplayTags.CombatState_Directional_Forward = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Directional.Forward"), FString("CombatState Directional Forward"));
	GameplayTags.CombatState_Directional_ForwardUp = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Directional.ForwardUp"), FString("CombatState Directional ForwardUp"));
	GameplayTags.CombatState_Directional_ForwardDown = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Directional.ForwardDown"), FString("CombatState Directional ForwardDown"));
	
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Unoccupied);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Ladder);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Falling);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Rope);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Ledge);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Climbing);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Hopping);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Hook);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Slope);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Entangled);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Swimming);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Crouching);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_UnCrouching);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Dodging);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Rolling);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Elevator);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Walking);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_WalkingPeaceful);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Aiming);
	
	GameplayTags.CombatDirections.AddTag(GameplayTags.CombatState_Directional_Upward);
	GameplayTags.CombatDirections.AddTag(GameplayTags.CombatState_Directional_Downward);
	GameplayTags.CombatDirections.AddTag(GameplayTags.CombatState_Directional_Forward);
	GameplayTags.CombatDirections.AddTag(GameplayTags.CombatState_Directional_ForwardUp);
	GameplayTags.CombatDirections.AddTag(GameplayTags.CombatState_Directional_ForwardDown);

	/*
	 * Unique States
	 */
	GameplayTags.Invincibility = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Invincibility"), FString("Invincibility"));
	GameplayTags.Shackled = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Shackled"), FString("Shackled"));
	GameplayTags.Defeated = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Defeated"), FString("Defeated"));
	GameplayTags.Peaceful = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Peaceful"), FString("Peaceful"));
	
	/*
	 * Gameplay Cues
	 */
	GameplayTags.GameplayCue_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.FireBlast"), FString("Gameplay Cue FireBlast"));
	GameplayTags.GameplayCue_Healing_Cure_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Healing.Cure.1"), FString("GameplayCue Healing Cure 1"));
	GameplayTags.GameplayCue_Healing_Cure_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Healing.Cure.2"), FString("GameplayCue Healing Cure 2"));
	GameplayTags.GameplayCue_Lightning_Bolt_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Lightning.Bolt.1"), FString("GameplayCue Lightning Bolt 1"));
	GameplayTags.GameplayCue_Noxious_Bio_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Noxious.Bio.1"), FString("GameplayCue Noxious Bio 1"));
	GameplayTags.GameplayCue_Cosmos_Meteor_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Cosmos.Meteor.1"), FString("GameplayCue Cosmos Meteor 1"));
	GameplayTags.GameplayCue_Weapon_Sword_Slash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Weapon.Sword.Slash"), FString("GameplayCue Weapon Sword Slash"));
	GameplayTags.GameplayCue_Alteration_Berserk_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Alteration.Berserk.1"), FString("GameplayCue Alteration Berserk 1"));
	GameplayTags.GameplayCue_Execute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Execute"), FString("GameplayCue Execute"));
	
	/*
	 * Abilities Cooldown
	 */
	GameplayTags.Cooldown_Throw_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Throw.1"), FString("Cooldown Throw 1"));
	GameplayTags.Cooldown_Throw_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Throw.2"), FString("Cooldown Throw 2"));
	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBolt"), FString("Fire Elemental Ability FireBolt"));
	GameplayTags.Cooldown_Healing_Cure_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Healing.Cure.1"), FString("Cooldown Healing Cure 1"));
	GameplayTags.Cooldown_Healing_Cure_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Healing.Cure.2"), FString("Cooldown Healing Cure 2"));
	GameplayTags.Cooldown_Lightning_Bolt_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Lightning.Bolt.1"), FString("Cooldown Lightning Bolt 1"));
	GameplayTags.Cooldown_Noxious_Bio_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Noxious.Bio.1"), FString("Cooldown Noxious Bio 1"));
	GameplayTags.Cooldown_Cosmos_Meteor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Cosmos.Meteor"), FString("Cooldown Cosmos Meteor"));
	GameplayTags.Cooldown_Weapon_Sword_Slash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Weapon.Sword.Slash"), FString("Cooldown Weapon Sword Slash"));
	GameplayTags.Cooldown_Weapon_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Weapon.Fire.FireBolt"), FString("Cooldown Fire FireBolt Weapon Version"));
	GameplayTags.Cooldown_Alteration_Berserk_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Alteration.Berserk.1"), FString("Cooldown Alteration Berserk 1"));
	GameplayTags.Cooldown_Weapon_Bow_Shot = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Weapon.Bow.Shot"), FString("Cooldown Weapon Bow Shot"));
	
	/*
	 * Side Effects
	 */
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Hit Reacting Effect"));
		
	/*
	 * Combat Sockets
	 */
	GameplayTags.CombatSocket_Preferred = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Preferred"), FString("Attack with Preferred Socket"));
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"), FString("Attack with Weapon Socket"));
	GameplayTags.CombatSocket_Weapon_UpperBody = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon.UpperBody"), FString("Attack with UpperBody Weapon Socket"));
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftHand"), FString("Attack with Left Hand Socket"));
	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightHand"), FString("Attack with Right Hand Socket"));
	GameplayTags.CombatSocket_Fang = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Fang"), FString("Attack with Fang Socket"));
	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Tail"), FString("Attack with Tail Socket"));
	GameplayTags.CombatSocket_LeftFeet = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftFeet"), FString("Attack with Left Feet Socket"));
	GameplayTags.CombatSocket_RightFeet = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightFeet"), FString("Attack with Right Feet Socket"));

	/*
	 * Items
	 */
	GameplayTags.Message_Potion_Health_Instant = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.Potion.Health.Instant"), FString("Message Potion Health Instant"));
	GameplayTags.Message_Potion_Health_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.Potion.Health.Duration"), FString("Message Potion Health Duration"));
	GameplayTags.Message_Potion_Mana_Instant = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.Potion.Mana.Instant"), FString("Message Potion Mana Instant"));
	GameplayTags.Message_Potion_Mana_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.Potion.Mana.Duration"), FString("Message Potion Mana Duration"));
	
	/*
	 * Indicators
	 */
	GameplayTags.Indicator_Execute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Indicator.Execute"), FString("Indicator Execute"));
	GameplayTags.Indicator_Execute_PercentMissingHealth_Flat = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Indicator.Execute.PercentMissingHealth.Flat"), FString("Indicator Execute PercentMissingHealth Flat"));
	GameplayTags.Indicator_Execute_PercentMissingHealth_Scaling = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Indicator.Execute.PercentMissingHealth.Scaling"), FString("Indicator Execute PercentMissingHealth Scaling"));
	GameplayTags.Indicator_Execute_PercentMissingHealth_Stack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Indicator.Execute.PercentMissingHealth.Stack"), FString("Indicator Execute PercentMissingHealth Stack"));
	GameplayTags.Indicator_Entangled = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Indicator.Entangled"), FString("Indicator Entangled"));
	GameplayTags.Indicator_Bleeding = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Indicator.Bleeding"), FString("Indicator Bleeding"));
	
	/*
	 * Montages / GameplayEvents
	 */
	GameplayTags.Event_Montage_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.FireBolt"), FString("Event Montage FireBolt"));
	GameplayTags.Event_Montage_Bow_Draw = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Bow.Draw"), FString("Event Montage Bow Draw"));
	GameplayTags.Event_Montage_Bow_Shot = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Bow.Shot"), FString("Event Montage Bow Shot"));
	
	/*
	 * Montages
	 */
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"), FString("Montage Attack 1"));
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"), FString("Montage Attack 2"));
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"), FString("Montage Attack 3"));
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"), FString("Montage Attack 4"));
	GameplayTags.Montage_Crouch_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Crouch.Attack"), FString("Montage Crouch Attack"));
	GameplayTags.Montage_Jump_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Jump.Attack"), FString("Montage Jump Attack"));
	GameplayTags.Montage_Falling_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Falling.Attack"), FString("Montage Falling Attack"));
	GameplayTags.Montage_Ascent_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Ascent.Attack"), FString("Montage Ascent Attack"));
	GameplayTags.Montage_Upward_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Upward.Attack"), FString("Montage Upward Attack"));
	GameplayTags.Montage_Rope_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Rope.Attack"), FString("Montage Rope Attack"));
	GameplayTags.Montage_Slope_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Slope.Attack"), FString("Montage Slope Attack"));
	GameplayTags.Montage_Ladder_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Ladder.Attack"), FString("Montage Ladder Attack"));
	GameplayTags.Montage_Roll_End = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Roll.End"), FString("Montage Roll End"));
	GameplayTags.Montage_Roll_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Roll.Attack"), FString("Montage Roll Attack"));
	GameplayTags.Montage_Swim_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Swim.Attack"), FString("Montage Swim Attack"));
	GameplayTags.Montage_Dodge_End = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Dodge.End"), FString("Montage Dodge End"));
	GameplayTags.Montage_EndAbility = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.EndAbility"), FString("Montage End Ability"));
	GameplayTags.Montage_Parry = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Parry"), FString("Montage Parry"));
	GameplayTags.Montage_Parry_Begin = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Parry.Begin"), FString("Montage Parry Begin"));
	GameplayTags.Montage_Parry_End = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Parry.End"), FString("Montage Parry End"));
	
	GameplayTags.Montage_Window_Combo = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Window.Combo"), FString("Montage Window Combo"));
	GameplayTags.Montage_Window_Execute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Window.Execute"), FString("Montage Window Execute"));
	
	/*
	 * Input Tags
	 */
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("Left Mouse Button"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("Right Mouse Button"));
	GameplayTags.InputTag_Shoulder_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Shoulder.Left"), FString("Left Shoulder Button"));
	GameplayTags.InputTag_Shoulder_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Shoulder.Right"), FString("Right Shoulder Button"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString("1"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString("2"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), FString("3"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), FString("4"));
	GameplayTags.InputTag_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.5"), FString("5"));
	GameplayTags.InputTag_6 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.6"), FString("6"));
	GameplayTags.InputTag_7 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.7"), FString("7"));
	GameplayTags.InputTag_8 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.8"), FString("8"));
	GameplayTags.InputTag_9 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.9"), FString("9"));
	GameplayTags.InputTag_0 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.0"), FString("0"));
	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.1"), FString("Passive 1"));
	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.2"), FString("Passive 2"));
	GameplayTags.InputTag_Passive_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.3"), FString("Passive 3"));
	GameplayTags.InputTag_Passive_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.4"), FString("Passive 4"));
	GameplayTags.InputTag_Passive_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.5"), FString("Passive 5"));	
	GameplayTags.InputTag_Throw_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Throw.1"), FString("Throw 1"));
	GameplayTags.InputTag_Throw_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Throw.2"), FString("Throw 2"));

	/*
	* Player Tags
	*/
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputPressed"), FString("Block input pressed from player"));
	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputHeld"), FString("Block input pressed from player"));
	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputReleased"), FString("Block input pressed from player"));
	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.CursorTrace"), FString("Block trace under cursor"));
}
