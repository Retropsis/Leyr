// @ Retropsis 2024-2025.

#include "Game/BaseGameplayTags.h"
#include "GameplayTagsManager.h"

FBaseGameplayTags FBaseGameplayTags::GameplayTags;

void FBaseGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Unique States
	 */
	GameplayTags.Player = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player"), FString("Player"));
	GameplayTags.Player_First = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.First"), FString("Player First"));
	GameplayTags.Player_Second = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Second"), FString("Player Second"));
	GameplayTags.Invincibility = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Invincibility"), FString("Invincibility"));
	GameplayTags.Shackled = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Shackled"), FString("Shackled"));
	GameplayTags.Defeated = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Defeated"), FString("Defeated"));
	GameplayTags.Peaceful = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Peaceful"), FString("Peaceful"));
	GameplayTags.Poise = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Poise"), FString("Poise"));
	GameplayTags.Execute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Execute"), FString("Execute"));
	
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
	GameplayTags.Attributes_Secondary_Poise = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Poise"), FString("Amount of balance using abilities"));
	
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
	GameplayTags.StatusEffect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.HitReact"), FString("Hit Reacting Status Effect"));
	GameplayTags.StatusEffect_HitReact_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.HitReact.1"), FString("First Hit Reacting Status Effect"));
	GameplayTags.StatusEffect_HitReact_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.HitReact.2"), FString("Second Hit Reacting Status Effect"));
	GameplayTags.StatusEffect_Weakened = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Weakened"), FString("Weakened Status Effect"));
	GameplayTags.StatusEffect_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Burn"), FString("Status Effect from Fire"));
	GameplayTags.StatusEffect_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Stun"), FString("Status Effect from Ice"));
	GameplayTags.StatusEffect_Sleep = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Sleep"), FString("Status Effect"));
	GameplayTags.StatusEffect_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Poison"), FString("Status Effect"));
	GameplayTags.StatusEffect_Silence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Silence"), FString("Status Effect"));
	GameplayTags.StatusEffect_Bleed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Bleed"), FString("Status Effect"));

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
	GameplayTags.DamageTypesToStatusEffects.Add(GameplayTags.Damage_Cutting, GameplayTags.StatusEffect_Bleed);

	GameplayTags.StatusEffectsToDamageTypes.Add(GameplayTags.StatusEffect_Burn, GameplayTags.Damage_Fire);
	GameplayTags.StatusEffectsToDamageTypes.Add(GameplayTags.StatusEffect_Stun, GameplayTags.Damage_Blunt);
	GameplayTags.StatusEffectsToDamageTypes.Add(GameplayTags.StatusEffect_Silence, FGameplayTag());
	GameplayTags.StatusEffectsToDamageTypes.Add(GameplayTags.StatusEffect_Sleep, FGameplayTag());
	GameplayTags.StatusEffectsToDamageTypes.Add(GameplayTags.StatusEffect_Poison, GameplayTags.Damage_Noxious);
	GameplayTags.StatusEffectsToDamageTypes.Add(GameplayTags.StatusEffect_Bleed, GameplayTags.Damage_Cutting);
	
	GameplayTags.StatusEffectsToResistances.Add(GameplayTags.StatusEffect_Burn, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.StatusEffectsToResistances.Add(GameplayTags.StatusEffect_Stun, GameplayTags.Attributes_Resistance_Blunt);
	GameplayTags.StatusEffectsToResistances.Add(GameplayTags.StatusEffect_Silence, GameplayTags.Attributes_Resistance_Ice);
	GameplayTags.StatusEffectsToResistances.Add(GameplayTags.StatusEffect_Sleep, GameplayTags.Attributes_Resistance_Wind);
	GameplayTags.StatusEffectsToResistances.Add(GameplayTags.StatusEffect_Poison, GameplayTags.Attributes_Resistance_Noxious);
	GameplayTags.StatusEffectsToResistances.Add(GameplayTags.StatusEffect_Bleed, GameplayTags.Attributes_Resistance_Cutting);
	
	GameplayTags.StatusEffectToPeriodicEffectOnApplicationPolicy.Add(GameplayTags.StatusEffect_Stun, true);
	GameplayTags.StatusEffectToPeriodicEffectOnApplicationPolicy.Add(GameplayTags.StatusEffect_Silence, true);
	GameplayTags.StatusEffectToPeriodicEffectOnApplicationPolicy.Add(GameplayTags.StatusEffect_Sleep, true);
	GameplayTags.StatusEffectToPeriodicEffectOnApplicationPolicy.Add(GameplayTags.StatusEffect_Poison, false);
	GameplayTags.StatusEffectToPeriodicEffectOnApplicationPolicy.Add(GameplayTags.StatusEffect_Burn, false);
	GameplayTags.StatusEffectToPeriodicEffectOnApplicationPolicy.Add(GameplayTags.StatusEffect_Bleed, false);
		
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
	GameplayTags.Abilities_Airborne = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Airborne"), FString("Melee Airborne"));
	GameplayTags.Abilities_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Dodge"), FString("Abilities Dodge"));
	GameplayTags.Abilities_Roll = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Roll"), FString("Abilities Roll"));
	GameplayTags.Abilities_Parry = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Parry"), FString("Melee Parry"));
	GameplayTags.Abilities_Block = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Block"), FString("Melee Block"));
	GameplayTags.Abilities_Shield = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Shield"), FString("Melee Shield"));
	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Summon"), FString("Ability Summon"));
	
	GameplayTags.Abilities_ClimbingRope = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.ClimbingRope"), FString("Ability ClimbingRope"));
	GameplayTags.Abilities_SwimImpulse = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.SwimImpulse"), FString("Ability SwimImpulse"));
	GameplayTags.Abilities_RootMotion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.RootMotion"), FString("Ability RootMotion"));
	GameplayTags.Abilities_Leap = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Leap"), FString("Ability Leap"));
	
	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBolt"), FString("Fire Element Ability FireBolt"));
	GameplayTags.Abilities_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBlast"), FString("Fire Element Ability FireBlast"));
	GameplayTags.Abilities_Fire_FireBreath = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBreath"), FString("Fire Element Ability FireBreath"));
	GameplayTags.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lightning.Electrocute"), FString("Lightning Element Electrocute Ability"));
	GameplayTags.Abilities_Lightning_Bolt_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lightning.Bolt.1"), FString("Abilities Lightning Bolt 1"));
	GameplayTags.Abilities_Noxious_Bio_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Noxious.Bio.1"), FString("Abilities Noxious Bio 1"));
	GameplayTags.Abilities_Noxious_Dart = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Noxious.Dart"), FString("Abilities Noxious Dart"));
	GameplayTags.Abilities_Noxious_Orb = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Noxious.Orb"), FString("Abilities Noxious Orb"));
	GameplayTags.Abilities_Cosmos_Meteor_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Cosmos.Meteor.1"), FString("Abilities Cosmos Meteor 1"));
	GameplayTags.Abilities_Earth_Earthquake = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Earth.Earthquake"), FString("Earth Element Earthquake Ability"));
	GameplayTags.Abilities_Sorcery = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Sorcery"), FString("Sorcery Ability"));

	GameplayTags.Abilities_Cure_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Cure.1"), FString("Abilities Cure 1"));
	GameplayTags.Abilities_Cure_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Cure.2"), FString("Abilities Cure 2"));
	
	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.HitReact"), FString("HitReact Ability Effect"));
	
	GameplayTags.Abilities_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive"), FString("Passive Ability"));
	GameplayTags.Abilities_Passive_ListenForEvent = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.ListenForEvent"), FString("Passive Ability ListenForEvent"));
	GameplayTags.Abilities_Passive_Mastery = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.Mastery"), FString("Passive Ability Mastery"));
	GameplayTags.Abilities_Passive_Mastery_Sword = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.Mastery.Sword"), FString("Sword Mastery"));
	GameplayTags.Abilities_Passive_Mastery_Dagger = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.Mastery.Dagger"), FString("Dagger Mastery"));
	GameplayTags.Abilities_Passive_Mastery_Axe = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.Mastery.Axe"), FString("Axe Mastery"));
	GameplayTags.Abilities_Passive_Mastery_Blunt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.Mastery.Blunt"), FString("Blunt Mastery"));
	GameplayTags.Abilities_Passive_Mastery_DualWield = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.Mastery.DualWield"), FString("DualWield Mastery"));
	GameplayTags.Abilities_Passive_Mastery_GreatSword = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.Mastery.GreatSword"), FString("GreatSword Mastery"));
	GameplayTags.Abilities_Passive_Mastery_Bow = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.Mastery.Bow"), FString("Bow Mastery"));
	GameplayTags.Abilities_Passive_Mastery_Crossbow = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.Mastery.Crossbow"), FString("Crossbow Mastery"));
	GameplayTags.Abilities_Passive_Mastery_Buckler = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.Mastery.Buckler"), FString("Buckler Mastery"));
	GameplayTags.Abilities_Passive_Mastery_Sorcery = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.Mastery.Sorcery"), FString("Sorcery Mastery"));
	
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
	GameplayTags.Abilities_Type_ActionSlot = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.ActionSlot"), FString("Abilities Type ActionSlot"));
	GameplayTags.Abilities_Type_ActionSlot_Main = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.ActionSlot.Main"), FString("Abilities Type ActionSlot Main"));
	GameplayTags.Abilities_Type_ActionSlot_Sub = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.ActionSlot.Sub"), FString("Abilities Type ActionSlot Sub"));
	GameplayTags.Abilities_Type_PassiveSlot = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.PassiveSlot"), FString("Abilities Type PassiveSlot"));

	/*
	 * Fighting
	 */
	GameplayTags.Abilities_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon"), FString("Abilities Weapon"));
	GameplayTags.Abilities_Weapon_Monk = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Monk"), FString("Abilities Weapon Monk"));
	GameplayTags.Abilities_Weapon_Monk_States = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Monk.States"), FString("Abilities Weapon Monk States"));
	GameplayTags.Abilities_Weapon_Monk_Combo = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Monk.Combo"), FString("Abilities Weapon Monk Combo"));
	GameplayTags.Abilities_Weapon_Monk_Extra = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Monk.Extra"), FString("Abilities Weapon Monk Extra"));
	GameplayTags.Abilities_Weapon_Monk_Execute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Monk.Execute"), FString("Abilities Weapon Monk Execute"));
	
	/*
	 * Melee Weapons
	 */
	GameplayTags.Abilities_Weapon_Sword_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Sword.1"), FString("Abilities Weapon Sword 1"));
	GameplayTags.Abilities_Weapon_Sword_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Sword.2"), FString("Abilities Weapon Sword 2"));
	GameplayTags.Abilities_Weapon_Sword_Slash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Sword.Slash"), FString("Abilities Weapon Sword Slash"));
	
	GameplayTags.Abilities_Weapon_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Fire.FireBolt"), FString("Abilities Fire FireBolt Weapon Version"));
	GameplayTags.Abilities_Weapon_Noxious_Bio_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Noxious.Bio.1"), FString("Abilities Weapon Noxious Bio 1"));
	
	GameplayTags.Abilities_Weapon_OneHanded_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.OneHanded.Attack"), FString("Abilities Weapon OneHanded Attack"));
	GameplayTags.Abilities_Weapon_OneHanded_States = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.OneHanded.States"), FString("Abilities Weapon OneHanded States"));
	GameplayTags.Abilities_Weapon_OneHanded_Combo = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.OneHanded.Combo"), FString("Abilities Weapon OneHanded Combo"));
	GameplayTags.Abilities_Weapon_OneHanded_Extra = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.OneHanded.Extra"), FString("Abilities Weapon OneHanded Extra"));
	GameplayTags.Abilities_Weapon_OneHanded_Execute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.OneHanded.Execute"), FString("Abilities Weapon OneHanded Execute"));
	GameplayTags.Abilities_Weapon_TwoHanded_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.TwoHanded.Attack"), FString("Abilities Weapon TwoHanded Attack"));
	GameplayTags.Abilities_Weapon_TwoHanded_States = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.TwoHanded.States"), FString("Abilities Weapon TwoHanded States"));
	GameplayTags.Abilities_Weapon_TwoHanded_Combo = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.TwoHanded.Combo"), FString("Abilities Weapon TwoHanded Combo"));
	GameplayTags.Abilities_Weapon_TwoHanded_Extra = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.TwoHanded.Extra"), FString("Abilities Weapon TwoHanded Extra"));
	GameplayTags.Abilities_Weapon_TwoHanded_Execute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.TwoHanded.Execute"), FString("Abilities Weapon TwoHanded Execute"));
	GameplayTags.Abilities_Weapon_Whip_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Whip.Attack"), FString("Abilities Weapon Whip Attack"));

	/*
	 * Range Weapons
	 */
	GameplayTags.Abilities_Weapon_Aim = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Aim"), FString("Abilities Weapon Aim"));
	GameplayTags.Abilities_Weapon_Bow_Shot = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Bow.Shot"), FString("Abilities Weapon Bow Shot"));
	GameplayTags.Abilities_Weapon_Throw_Spear = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Throw.Spear"), FString("Abilities Weapon Spear Throw"));
	GameplayTags.Abilities_Weapon_Throw_Kunai = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Throw.Kunai"), FString("Abilities Weapon Kunai Throw"));
	GameplayTags.Abilities_Weapon_Throw_Boomerang = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Throw.Boomerang"), FString("Abilities Weapon Boomerang Throw"));
	GameplayTags.Abilities_Weapon_Throw_Knife = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Throw.Knife"), FString("Abilities Weapon Knife Throw"));
	
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
	GameplayTags.Abilities_Combo_Window_Cue = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Combo.Window.Cue"), FString("Abilities Combo Window Cue"));
	GameplayTags.Abilities_Combo_Window_End = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Combo.Window.End"), FString("Abilities Combo Window End"));
	GameplayTags.Abilities_Combo_Window_Execute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Combo.Window.Execute"), FString("Abilities Combo Window Execute"));

	/*
	 * AI Abilities
	 */
	GameplayTags.Abilities_JumpToLocation = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.JumpToLocation"), FString("Abilities JumpToLocation"));
	GameplayTags.Abilities_Range_GroundBurst = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Range.GroundBurst"), FString("Abilities Range GroundBurst"));
	
	/*
	 * Combat States
	 */
	GameplayTags.CombatState_Unoccupied = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Unoccupied"), FString("CombatState Unoccupied"));
	GameplayTags.CombatState_Defeated = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Defeated"), FString("CombatState Defeated"));
	GameplayTags.CombatState_Peaceful = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Peaceful"), FString("CombatState Peaceful"));
	GameplayTags.CombatState_Elevator = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Elevator"), FString("CombatState Elevator"));
	GameplayTags.CombatState_UnCrouching = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.UnCrouching"), FString("CombatState UnCrouching"));
	GameplayTags.CombatState_Condition_Slope = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Condition.Slope"), FString("CombatState Condition Slope"));
	GameplayTags.CombatState_Condition_Ledge = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Condition.Ledge"), FString("CombatState Condition Ledge"));
	GameplayTags.CombatState_Condition_Rope = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Condition.Rope"), FString("CombatState Condition Rope"));
	GameplayTags.CombatState_Condition_Hook = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Condition.Hook"), FString("CombatState Condition Hook"));
	GameplayTags.CombatState_Condition_Ladder = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Condition.Ladder"), FString("CombatState Condition Ladder"));
	GameplayTags.CombatState_Condition_Climbing = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Condition.Climbing"), FString("CombatState Condition Climbing"));
	GameplayTags.CombatState_Condition_Falling = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Condition.Falling"), FString("CombatState Condition Falling"));
	GameplayTags.CombatState_Condition_Crouching = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Condition.Crouching"), FString("CombatState Condition Crouching"));
	GameplayTags.CombatState_Condition_Swimming = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Condition.Swimming"), FString("CombatState Condition Swimming"));
	GameplayTags.CombatState_Condition_Entangled = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Condition.Entangled"), FString("CombatState Condition Entangled"));
	GameplayTags.CombatState_Condition_Walking = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Condition.Walking"), FString("CombatState Condition Walking"));
	GameplayTags.CombatState_Transient_Aiming = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Transient.Aiming"), FString("CombatState Transient Aiming"));
	GameplayTags.CombatState_Transient_Rope = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Transient.Rope"), FString("CombatState Transient Rope"));
	GameplayTags.CombatState_Transient_Ledge = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Transient.Ledge"), FString("CombatState Transient Ledge"));
	GameplayTags.CombatState_Transient_HitReacting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Transient.HitReacting"), FString("CombatState Transient HitReacting"));
	GameplayTags.CombatState_Transient_Dodging = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Transient.Dodging"), FString("CombatState Transient Dodging"));
	GameplayTags.CombatState_Transient_Rolling = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Transient.Rolling"), FString("CombatState Transient Rolling"));
	
	GameplayTags.CombatState_Execute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Execute"), FString("CombatState Execute"));
	
	GameplayTags.CombatState_Directional_Upward = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Directional.Upward"), FString("CombatState Directional Upward"));
	GameplayTags.CombatState_Directional_Downward = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Directional.Downward"), FString("CombatState Directional Downward"));
	GameplayTags.CombatState_Directional_Forward = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Directional.Forward"), FString("CombatState Directional Forward"));
	GameplayTags.CombatState_Directional_ForwardUp = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Directional.ForwardUp"), FString("CombatState Directional ForwardUp"));
	GameplayTags.CombatState_Directional_ForwardDown = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Directional.ForwardDown"), FString("CombatState Directional ForwardDown"));

	GameplayTags.CombatState_Rule = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Rule"), FString("CombatState Rule"));
	GameplayTags.CombatState_Rule_Block = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Rule.Block"), FString("CombatState Rule Block"));
	GameplayTags.CombatState_Rule_Block_All = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Rule.Block.All"), FString("CombatState Rule Block All"));
	GameplayTags.CombatState_Rule_Block_Movement = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Rule.Block.Movement"), FString("CombatState Rule Block Movement"));
	GameplayTags.CombatState_Rule_Block_Ability = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Rule.Block.Ability"), FString("CombatState Rule Block Ability"));
	GameplayTags.CombatState_Rule_Block_Direction = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Rule.Block.Direction"), FString("CombatState Rule Block Direction"));
	GameplayTags.CombatState_Rule_Stop = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Rule.Stop"), FString("CombatState Rule Stop"));
	GameplayTags.CombatState_Rule_Stop_All = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Rule.Stop.All"), FString("CombatState Rule Stop All"));
	GameplayTags.CombatState_Rule_Stop_Movement = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Rule.Stop.Movement"), FString("CombatState Rule Stop Movement"));
	GameplayTags.CombatState_Rule_Stop_Ability = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatState.Rule.Stop.Ability"), FString("CombatState Rule Stop Ability"));
	
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Unoccupied);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Defeated);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Peaceful);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Elevator);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_UnCrouching);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Condition_Walking);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Condition_Ladder);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Condition_Falling);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Condition_Rope);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Condition_Ledge);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Condition_Climbing);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Condition_Hook);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Condition_Slope);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Condition_Entangled);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Condition_Swimming);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Condition_Crouching);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Transient_Rope);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Transient_Ledge);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Transient_Dodging);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Transient_Rolling);
	GameplayTags.CombatStates.AddTag(GameplayTags.CombatState_Transient_Aiming);
	
	GameplayTags.CombatDirections.AddTag(GameplayTags.CombatState_Directional_Upward);
	GameplayTags.CombatDirections.AddTag(GameplayTags.CombatState_Directional_Downward);
	GameplayTags.CombatDirections.AddTag(GameplayTags.CombatState_Directional_Forward);
	GameplayTags.CombatDirections.AddTag(GameplayTags.CombatState_Directional_ForwardUp);
	GameplayTags.CombatDirections.AddTag(GameplayTags.CombatState_Directional_ForwardDown);
	
	GameplayTags.ToPreviousStateFilter.AddTag(GameplayTags.CombatState_Elevator);
	GameplayTags.ToPreviousStateFilter.AddTag(GameplayTags.CombatState_Condition_Ladder);
	GameplayTags.ToPreviousStateFilter.AddTag(GameplayTags.CombatState_Condition_Rope);
	GameplayTags.ToPreviousStateFilter.AddTag(GameplayTags.CombatState_Condition_Ledge);
	GameplayTags.ToPreviousStateFilter.AddTag(GameplayTags.CombatState_Condition_Climbing);
	GameplayTags.ToPreviousStateFilter.AddTag(GameplayTags.CombatState_Condition_Hook);
	GameplayTags.ToPreviousStateFilter.AddTag(GameplayTags.CombatState_Condition_Slope);
	GameplayTags.ToPreviousStateFilter.AddTag(GameplayTags.CombatState_Condition_Entangled);
	GameplayTags.ToPreviousStateFilter.AddTag(GameplayTags.CombatState_Condition_Swimming);
	GameplayTags.ToPreviousStateFilter.AddTag(GameplayTags.CombatState_Condition_Crouching);
	
	GameplayTags.ToUnoccupiedStateFilter.AddTag(GameplayTags.CombatState_Condition_Falling);
	GameplayTags.ToUnoccupiedStateFilter.AddTag(GameplayTags.CombatState_Transient_Rope);
	GameplayTags.ToUnoccupiedStateFilter.AddTag(GameplayTags.CombatState_Transient_Ledge);
	GameplayTags.ToUnoccupiedStateFilter.AddTag(GameplayTags.CombatState_Transient_Dodging);
	GameplayTags.ToUnoccupiedStateFilter.AddTag(GameplayTags.CombatState_Transient_Rolling);
	GameplayTags.ToUnoccupiedStateFilter.AddTag(GameplayTags.CombatState_Transient_Aiming);

	GameplayTags.CombatStateRules.AddTag(GameplayTags.CombatState_Rule_Block_All);
	GameplayTags.CombatStateRules.AddTag(GameplayTags.CombatState_Rule_Block_Movement);
	GameplayTags.CombatStateRules.AddTag(GameplayTags.CombatState_Rule_Block_Ability);
	GameplayTags.CombatStateRules.AddTag(GameplayTags.CombatState_Rule_Stop_All);
	GameplayTags.CombatStateRules.AddTag(GameplayTags.CombatState_Rule_Stop_Movement);
	GameplayTags.CombatStateRules.AddTag(GameplayTags.CombatState_Rule_Stop_Ability);
	
	/*
	 * Gameplay Cues
	 */
	GameplayTags.GameplayCue_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.FireBlast"), FString("Gameplay Cue FireBlast"));
	GameplayTags.GameplayCue_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Electrocute"), FString("Gameplay Cue Electrocute"));
	GameplayTags.GameplayCue_Electrocute_Loop = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Earth.Earthquake"), FString("Gameplay Cue Earth Earthquake"));
	GameplayTags.GameplayCue_Earth_Earthquake = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Electrocute.Loop"), FString("Gameplay Cue Electrocute Loop"));
	GameplayTags.GameplayCue_Healing_Cure_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Healing.Cure.1"), FString("GameplayCue Healing Cure 1"));
	GameplayTags.GameplayCue_Healing_Cure_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Healing.Cure.2"), FString("GameplayCue Healing Cure 2"));
	GameplayTags.GameplayCue_Lightning_Bolt_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Lightning.Bolt.1"), FString("GameplayCue Lightning Bolt 1"));
	GameplayTags.GameplayCue_Noxious_Bio_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Noxious.Bio.1"), FString("GameplayCue Noxious Bio 1"));
	GameplayTags.GameplayCue_Cosmos_Meteor_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Cosmos.Meteor.1"), FString("GameplayCue Cosmos Meteor 1"));
	
	GameplayTags.GameplayCue_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Weapon"), FString("GameplayCue Weapon"));
	GameplayTags.GameplayCue_Weapon_Monk = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Weapon.Monk"), FString("GameplayCue Monk Weapon"));
	GameplayTags.GameplayCue_Weapon_Sword_Slash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Weapon.Sword.Slash"), FString("GameplayCue Weapon Sword Slash"));
	GameplayTags.GameplayCue_Weapon_Bow_Shot = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Bite"), FString("GameplayCue Bite"));
	GameplayTags.GameplayCue_Weapon_Impact_Ground = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Weapon.Impact.Ground"), FString("GameplayCue Weapon Ground Impact."));
	GameplayTags.GameplayCue_Weapon_Impact_Small = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Weapon.Impact.Small"), FString("GameplayCue Weapon Small Impact."));
	GameplayTags.GameplayCue_Weapon_Impact_Average_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Weapon.Impact.Average.1"), FString("GameplayCue Weapon Average Impact 1."));
	GameplayTags.GameplayCue_Weapon_Impact_Average_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Weapon.Impact.Average.2"), FString("GameplayCue Weapon Average Impact 2."));
	GameplayTags.GameplayCue_Weapon_Impact_Average_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Weapon.Impact.Average.3"), FString("GameplayCue Weapon Average Impact 3."));
	GameplayTags.GameplayCue_Weapon_Impact_Large = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Weapon.Impact.Large"), FString("GameplayCue Weapon Large Impact."));
	
	GameplayTags.GameplayCue_Wound_Impact_Random = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.Random"), FString("GameplayCue Wound Random Impact."));
	GameplayTags.GameplayCue_Wound_Impact_Small = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.Small"), FString("GameplayCue Wound Small Impact."));
	GameplayTags.GameplayCue_Wound_Impact_Small_Random = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.Small.Random"), FString("GameplayCue Wound Small Random Impact."));
	GameplayTags.GameplayCue_Wound_Impact_Small_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.Small.1"), FString("GameplayCue Wound Small 1 Impact."));
	GameplayTags.GameplayCue_Wound_Impact_Small_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.Small.2"), FString("GameplayCue Wound Small 2 Impact."));
	GameplayTags.GameplayCue_Wound_Impact_Small_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.Small.3"), FString("GameplayCue Wound Small 3 Impact."));
	GameplayTags.GameplayCue_Wound_Impact_Average = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.Average"), FString("GameplayCue Wound Average Impact."));
	GameplayTags.GameplayCue_Wound_Impact_Average_Random = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.Average.Random"), FString("GameplayCue Wound Average Random Impact."));
	GameplayTags.GameplayCue_Wound_Impact_Average_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.Average.1"), FString("GameplayCue Wound Average 1 Impact."));
	GameplayTags.GameplayCue_Wound_Impact_Average_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.Average.2"), FString("GameplayCue Wound Average 2 Impact."));
	GameplayTags.GameplayCue_Wound_Impact_Average_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.Average.3"), FString("GameplayCue Wound Average 3 Impact."));
	GameplayTags.GameplayCue_Wound_Impact_Large = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.Large"), FString("GameplayCue Wound Large Impact."));
	GameplayTags.GameplayCue_Wound_Impact_Large_Random = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.Large.Random"), FString("GameplayCue Wound Large Random Impact."));
	GameplayTags.GameplayCue_Wound_Impact_Large_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.Large.1"), FString("GameplayCue Wound Large 1 Impact."));
	GameplayTags.GameplayCue_Wound_Impact_Large_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.Large.2"), FString("GameplayCue Wound Large 2 Impact."));
	GameplayTags.GameplayCue_Wound_Impact_Large_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.Large.3"), FString("GameplayCue Wound Large 3 Impact."));
	GameplayTags.GameplayCue_Wound_Impact_AfterImage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Wound.Impact.AfterImage"), FString("GameplayCue Wound Impact AfterImage."));
	
	GameplayTags.GameplayCue_Bite = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Weapon.Bow.Shot"), FString("GameplayCue Weapon Bow Shot"));
	GameplayTags.GameplayCue_Execute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Execute"), FString("GameplayCue Execute"));
	GameplayTags.GameplayCue_Dodge_Dust = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Dodge.Dust"), FString("GameplayCue Dodge Dust"));
	GameplayTags.GameplayCue_Dodge_AfterImage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Dodge.AfterImage"), FString("GameplayCue Dodge AfterImage"));
	GameplayTags.GameplayCue_Slide_Dust = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Slide.Dust"), FString("GameplayCue Slide Dust"));
	GameplayTags.GameplayCue_Slide_AfterImage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Slide.AfterImage"), FString("GameplayCue Slide AfterImage"));
	GameplayTags.GameplayCue_Alteration_Berserk_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Alteration.Berserk.1"), FString("GameplayCue Alteration Berserk 1"));
	
	GameplayTags.GameplayCue_WaterSplash_Small = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.WaterSplash.Small"), FString("GameplayCue WaterSplash_Small"));
	GameplayTags.GameplayCue_WaterSplash_Regular = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.WaterSplash.Regular"), FString("GameplayCue WaterSplash_Regular"));
	GameplayTags.GameplayCue_WaterSplash_Large = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.WaterSplash.Large"), FString("GameplayCue WaterSplash_Large"));
	
	/*
	 * Asset Cues
	*/
	GameplayTags.AssetCue_ImpactEffect = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AssetCue.ImpactEffect"), FString("Gameplay Cue ImpactEffect"));
	GameplayTags.AssetCue_ImpactSound = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AssetCue.ImpactSound"), FString("GameplayCue ImpactSound"));
	
	/*
	 * Abilities Cooldown
	 */
	GameplayTags.Cooldown_Throw_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Throw.1"), FString("Cooldown Throw 1"));
	GameplayTags.Cooldown_Throw_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Throw.2"), FString("Cooldown Throw 2"));
	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBolt"), FString("Fire Elemental Ability FireBolt"));
	GameplayTags.Cooldown_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBlast"), FString("Fire Elemental Ability FireBlast"));
	GameplayTags.Cooldown_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Lightning.Electrocute"), FString("Elemental Ability Lightning Electrocute"));
	GameplayTags.Cooldown_Earth_Earthquake = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Earth.Earthquake"), FString("Elemental Ability Earth Earthquake"));
	GameplayTags.Cooldown_Healing_Cure_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Healing.Cure.1"), FString("Cooldown Healing Cure 1"));
	GameplayTags.Cooldown_Healing_Cure_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Healing.Cure.2"), FString("Cooldown Healing Cure 2"));
	GameplayTags.Cooldown_Lightning_Bolt_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Lightning.Bolt.1"), FString("Cooldown Lightning Bolt 1"));
	GameplayTags.Cooldown_Noxious_Bio_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Noxious.Bio.1"), FString("Cooldown Noxious Bio 1"));
	GameplayTags.Cooldown_Cosmos_Meteor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Cosmos.Meteor"), FString("Cooldown Cosmos Meteor"));
	GameplayTags.Cooldown_Weapon_Sword_Slash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Weapon.Sword.Slash"), FString("Cooldown Weapon Sword Slash"));
	GameplayTags.Cooldown_Weapon_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Weapon.Fire.FireBolt"), FString("Cooldown Fire FireBolt Weapon Version"));
	GameplayTags.Cooldown_Alteration_Berserk_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Alteration.Berserk.1"), FString("Cooldown Alteration Berserk 1"));
	GameplayTags.Cooldown_Weapon_Bow_Shot = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Weapon.Bow.Shot"), FString("Cooldown Weapon Bow Shot"));
	GameplayTags.Cooldown_Item_Instant = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Item.Instant"), FString("Cooldown Item Instant"));
	GameplayTags.Cooldown_Item_Short = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Item.Short"), FString("Cooldown Item Short"));
	GameplayTags.Cooldown_Item_Average = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Item.Average"), FString("Cooldown Item Average"));
	GameplayTags.Cooldown_Item_Long = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Item.Long"), FString("Cooldown Item Long"));
	GameplayTags.Cooldown_Item_VeryLong = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Item.VeryLong"), FString("Cooldown Item VeryLong"));
	GameplayTags.Cooldown_SwimImpulse = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.SwimImpulse"), FString("Cooldown SwimImpulse"));
		
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
	GameplayTags.Message_Pickup_Feather = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.Pickup.Feather"), FString("Message Pickup Feather"));
	GameplayTags.Message_Pickup_Skull = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.Pickup.Skull"), FString("Message Pickup Skull"));
	GameplayTags.Message_Pickup_Lantern = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.Pickup.Lantern"), FString("Message Pickup Lantern"));
	
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
	GameplayTags.Event_Montage_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Electrocute"), FString("Event Montage Electrocute"));
	GameplayTags.Event_Montage_Bow_Draw = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Bow.Draw"), FString("Event Montage Bow Draw"));
	GameplayTags.Event_Montage_Bow_Shot = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Bow.Shot"), FString("Event Montage Bow Shot"));
	
	/*
	 * Montages
	 */
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"), FString("Montage Attack 1"));
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"), FString("Montage Attack 2"));
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"), FString("Montage Attack 3"));
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"), FString("Montage Attack 4"));
	GameplayTags.Montage_Casting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Casting"), FString("Montage Casting"));
	GameplayTags.Montage_Shielding = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Shielding"), FString("Montage Shielding"));
	GameplayTags.Montage_Shielding_Begin = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Shielding.Begin"), FString("Montage Begin Shielding"));
	GameplayTags.Montage_Shielding_End = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Shielding.End"), FString("Montage End Shielding"));
	GameplayTags.Montage_Throwing = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Throwing"), FString("Montage Throwing"));
	GameplayTags.Montage_Using = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Using"), FString("Montage Using"));
	GameplayTags.Montage_Crouch_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Crouch.Attack"), FString("Montage Crouch Attack"));
	GameplayTags.Montage_Jump_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Jump.Attack"), FString("Montage Jump Attack"));
	GameplayTags.Montage_JumpForwardUp_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.JumpForwardUp.Attack"), FString("Montage Jump ForwardUp Attack"));
	GameplayTags.Montage_JumpForwardDown_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.JumpForwardDown.Attack"), FString("Montage Jump ForwardDown Attack"));
	GameplayTags.Montage_JumpUpward_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.JumpUpward.Attack"), FString("Montage Jump Upward Attack"));
	GameplayTags.Montage_ForwardUp_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.ForwardUp.Attack"), FString("Montage  ForwardUp Attack"));
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
	GameplayTags.Montage_Execute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Execute"), FString("Montage Execute"));
	GameplayTags.Montage_Execute_Success = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Execute.Success"), FString("Montage Execute Succeed"));
	GameplayTags.Montage_Execute_Failure = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Execute.Failure"), FString("Montage Execute Failed"));
	GameplayTags.Montage_HitStop = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.HitStop"), FString("Montage HitStop"));
	GameplayTags.Montage_Impulse = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Impulse"), FString("Montage Impulse"));
	
	GameplayTags.Montage_Cue = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Cue"), FString("Montage Cue"));
	GameplayTags.Montage_Cue_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Cue.1"), FString("Montage Cue 1"));
	GameplayTags.Montage_Cue_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Cue.2"), FString("Montage Cue 2"));
	GameplayTags.Montage_Cue_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Cue.3"), FString("Montage Cue 3"));
	GameplayTags.Montage_Cue_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Cue.4"), FString("Montage Cue 4"));
	
	GameplayTags.Montage_Window_Combo = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Window.Combo"), FString("Montage Window Combo"));
	GameplayTags.Montage_Window_Execute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Window.Execute"), FString("Montage Window Execute"));
		
	/*
	 * Equipment Tags
	 */
	GameplayTags.Equipment = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment"), FString("Equipment"));
	GameplayTags.Equipment_ActionSlot = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.ActionSlot"), FString("Equipment ActionSlot"));
	GameplayTags.Equipment_ActionSlot_0 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.ActionSlot.0"), FString("Equipment ActionSlot 0"));
	GameplayTags.Equipment_ActionSlot_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.ActionSlot.1"), FString("Equipment ActionSlot 1"));
	GameplayTags.Equipment_ActionSlot_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.ActionSlot.2"), FString("Equipment ActionSlot 2"));
	GameplayTags.Equipment_ActionSlot_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.ActionSlot.3"), FString("Equipment ActionSlot 3"));
	GameplayTags.Equipment_ActionSlot_MainHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.ActionSlot.MainHand"), FString("Equipment ActionSlot MainHand"));
	GameplayTags.Equipment_ActionSlot_OffHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.ActionSlot.OffHand"), FString("Equipment ActionSlot OffHand"));
	GameplayTags.Equipment_ActionSlot_Range = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.ActionSlot.Range"), FString("Equipment ActionSlot Range"));
	GameplayTags.Equipment_Headgear = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Headgear"), FString("Equipment Headgear"));
	GameplayTags.Equipment_Neck = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Neck"), FString("Equipment Neck"));
	GameplayTags.Equipment_UpperBody = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.UpperBody"), FString("Equipment UpperBody"));
	GameplayTags.Equipment_LowerBody = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.LowerBody"), FString("Equipment LowerBody"));
	GameplayTags.Equipment_Footwear = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Footwear"), FString("Equipment Footwear"));
	GameplayTags.Equipment_Wrist = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Wrist"), FString("Equipment Wrist"));
	GameplayTags.Equipment_Waist = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Waist"), FString("Equipment Waist"));
	GameplayTags.Equipment_Ammunition = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Ammunition"), FString("Equipment Ammunition"));
	GameplayTags.Equipment_Ammunition_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Ammunition.1"), FString("Equipment Ammunition 1"));
	GameplayTags.Equipment_Ammunition_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Ammunition.2"), FString("Equipment Ammunition 2"));
	GameplayTags.Equipment_Ammunition_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Equipment.Ammunition.3"), FString("Equipment Ammunition 3"));
	
	/*
	 * Inventory Cost Tags
	 */
	GameplayTags.Cost = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cost"), FString("Cost"));
	GameplayTags.Cost_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cost.None"), FString("Cost None"));
	GameplayTags.Cost_Arrow = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cost.Arrow"), FString("Cost Arrow"));
	GameplayTags.Cost_Bolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cost.Bolt"), FString("Cost Bolt"));
	GameplayTags.Cost_Bone = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cost.Bone"), FString("Cost Bone"));
	GameplayTags.Cost_Feather = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cost.Feather"), FString("Cost Feather"));
	
	/*
	 * Input Tags
	 */
	GameplayTags.InputTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag"), FString("Input Parent Tag"));
	GameplayTags.InputTag_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.None"), FString("No Input"));
	GameplayTags.InputTag_MainHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.MainHand"), FString("Main Hand Button"));
	GameplayTags.InputTag_OffHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.OffHand"), FString("OffHand Button"));
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
	GameplayTags.InputTag_Execute_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Execute.LMB"), FString("Execute LMB"));
	GameplayTags.InputTag_Execute_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Execute.RMB"), FString("Execute RMB"));
	GameplayTags.InputTag_Execute_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Execute.1"), FString("Execute 1"));

	// Input to Equipment
	GameplayTags.EquipmentSlotToInputTags.Add(GameplayTags.InputTag_MainHand, GameplayTags.Equipment_ActionSlot_MainHand);
	GameplayTags.EquipmentSlotToInputTags.Add(GameplayTags.InputTag_OffHand, GameplayTags.Equipment_ActionSlot_OffHand);
	GameplayTags.EquipmentSlotToInputTags.Add(GameplayTags.InputTag_1, GameplayTags.Equipment_ActionSlot_Range);
	// Equipment to Input
	GameplayTags.EquipmentSlotToInputTags.Add(GameplayTags.Equipment_ActionSlot_MainHand, GameplayTags.InputTag_MainHand);
	GameplayTags.EquipmentSlotToInputTags.Add(GameplayTags.Equipment_ActionSlot_OffHand, GameplayTags.InputTag_OffHand);
	GameplayTags.EquipmentSlotToInputTags.Add(GameplayTags.Equipment_ActionSlot_Range, GameplayTags.InputTag_1);
	// GameplayTags.EquipmentSlotToInputTags.Add(GameplayTags.Equipment_ActionSlot, GameplayTags.InputTag_1);
	
	// Ability to Input
	GameplayTags.AbilitySlotToInputTags.Add(GameplayTags.Abilities_Type_ActionSlot_Main, GameplayTags.InputTag_Shoulder_Right);
	GameplayTags.AbilitySlotToInputTags.Add(GameplayTags.Abilities_Type_ActionSlot_Sub, GameplayTags.InputTag_Execute_1);
	GameplayTags.AbilitySlotToInputTags.Add(GameplayTags.Abilities_Type_PassiveSlot, FGameplayTag{});

	/*
	* Player Tags
	*/
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputPressed"), FString("Block input pressed from player"));
	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputHeld"), FString("Block input pressed from player"));
	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputReleased"), FString("Block input pressed from player"));
	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.CursorTrace"), FString("Block trace under cursor"));
	
	/*
	* AI Pattern Tags
	*/
	GameplayTags.AI_Pattern = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern"), FString("AI Pattern"));
	GameplayTags.AI_Pattern_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.None"), FString("AI Pattern None"));
	GameplayTags.AI_Pattern_Random = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Random"), FString("AI Pattern Randomly"));
	GameplayTags.AI_Pattern_Phase1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase1"), FString("AI Pattern Phase 1"));
	GameplayTags.AI_Pattern_Phase1_Random = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase1.Random"), FString("AI Pattern Phase 1 Randomly"));
	GameplayTags.AI_Pattern_Phase1_A = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase1.A"), FString("AI Pattern Phase 1 Set A"));
	GameplayTags.AI_Pattern_Phase1_B = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase1.B"), FString("AI Pattern Phase 1 Set B"));
	GameplayTags.AI_Pattern_Phase1_C = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase1.C"), FString("AI Pattern Phase 1 Set C"));
	GameplayTags.AI_Pattern_Phase1_D = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase1.D"), FString("AI Pattern Phase 1 Set D"));
	GameplayTags.AI_Pattern_Phase2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase2"), FString("AI Pattern Phase 2"));
	GameplayTags.AI_Pattern_Phase2_Random = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase2.Random"), FString("AI Pattern Phase 2 Randomly"));
	GameplayTags.AI_Pattern_Phase2_A = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase2.A"), FString("AI Pattern Phase 2 Set A"));
	GameplayTags.AI_Pattern_Phase2_B = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase2.B"), FString("AI Pattern Phase 2 Set B"));
	GameplayTags.AI_Pattern_Phase2_C = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase2.C"), FString("AI Pattern Phase 2 Set C"));
	GameplayTags.AI_Pattern_Phase2_D = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase2.D"), FString("AI Pattern Phase 2 Set D"));
	GameplayTags.AI_Pattern_Phase3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase3"), FString("AI Pattern Phase 3"));
	GameplayTags.AI_Pattern_Phase3_Random = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase3.Random"), FString("AI Pattern Phase 3 Randomly"));
	GameplayTags.AI_Pattern_Phase3_A = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase3.A"), FString("AI Pattern Phase 3 Set A"));
	GameplayTags.AI_Pattern_Phase3_B = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase3.B"), FString("AI Pattern Phase 3 Set B"));
	GameplayTags.AI_Pattern_Phase3_C = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase3.C"), FString("AI Pattern Phase 3 Set C"));
	GameplayTags.AI_Pattern_Phase3_D = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("AI.Pattern.Phase3.D"), FString("AI Pattern Phase 3 Set D"));
	
	/*
	 * Spawn Positions
	*/
	GameplayTags.Spawn_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Spawn.Left"), FString("Spawn Left"));
	GameplayTags.Spawn_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Spawn.Right"), FString("Spawn Right"));
	GameplayTags.Spawn_Above = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Spawn.Above"), FString("Spawn Above"));
	GameplayTags.Spawn_Below = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Spawn.Below"), FString("Spawn Below"));

	/*
	 * Visual Effects
	 */
	GameplayTags.VisualEffect_HitReactFlash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("VisualEffect.HitReactFlash"), FString("VisualEffect HitReactFlash"));

	/*
	 * Encounter Spawns
	 */
	GameplayTags.EncounterSpawn_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EncounterSpawn.1"), FString("EncounterSpawn 1"));
	GameplayTags.EncounterSpawn_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EncounterSpawn.2"), FString("EncounterSpawn 2"));
	GameplayTags.EncounterSpawn_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EncounterSpawn.3"), FString("EncounterSpawn 3"));
	GameplayTags.EncounterSpawn_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EncounterSpawn.4"), FString("EncounterSpawn 4"));
	GameplayTags.EncounterSpawn_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EncounterSpawn.5"), FString("EncounterSpawn 5"));
	GameplayTags.EncounterSpawn_6 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EncounterSpawn.6"), FString("EncounterSpawn 6"));
	GameplayTags.EncounterSpawn_7 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EncounterSpawn.7"), FString("EncounterSpawn 7"));
	GameplayTags.EncounterSpawn_8 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EncounterSpawn.8"), FString("EncounterSpawn 8"));
	GameplayTags.EncounterSpawn_9 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EncounterSpawn.9"), FString("EncounterSpawn 9"));
}
