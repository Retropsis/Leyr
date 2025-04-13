// @ Retropsis 2024-2025.

#include "AbilityTypes.h"

bool FBaseGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid() )
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bIsBlockedHit)
		{
			RepBits |= 1 << 7;
		}
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 8;
		}
		if (bIsSuccessfulStatusEffect)
		{
			RepBits |= 1 << 9;
		}
		if (StatusEffectDamage > 0.f)
		{
			RepBits |= 1 << 10;
		}
		if (StatusEffectDuration > 0.f)
		{
			RepBits |= 1 << 11;
		}
		if (StatusEffectFrequency > 0.f)
		{
			RepBits |= 1 << 12;
		}
		if (DamageType.IsValid())
		{
			RepBits |= 1 << 13;
		}
		if (StatusEffectType.IsValid())
		{
			RepBits |= 1 << 14;
		}
		if (!DeathImpulse.IsZero())
		{
			RepBits |= 1 << 15;
		}
		if (!AirborneForce.IsZero())
		{
			RepBits |= 1 << 16;
		}
		if (bIsExecuteHit)
		{
			RepBits |= 1 << 17;
		}
		if (bIsRadialDamage)
		{
			RepBits |= 1 << 18;
 
			if (RadialDamageInnerRadius > 0.f)
			{
				RepBits |= 1 << 19;
			}
			if (RadialDamageOuterRadius > 0.f)
			{
				RepBits |= 1 << 20;
			}
			if (!RadialDamageOrigin.IsZero())
			{
				RepBits |= 1 << 21;
			}
		}
	}

	Ar.SerializeBits(&RepBits, 22);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	if (RepBits & (1 << 7))
	{
		Ar << bIsBlockedHit;
	}
	if (RepBits & (1 << 8))
	{
		Ar << bIsCriticalHit;
	}
	if (RepBits & (1 << 9))
	{
		Ar << bIsSuccessfulStatusEffect;
	}
	if (RepBits & (1 << 10))
	{
		Ar << StatusEffectDamage;
	}
	if (RepBits & (1 << 11))
	{
		Ar << StatusEffectDuration;
	}
	if (RepBits & (1 << 12))
	{
		Ar << StatusEffectFrequency;
	}
	if (RepBits & (1 << 13))
	{
		if (Ar.IsLoading())
		{
			if (!DamageType.IsValid())
			{
				DamageType = TSharedPtr<FGameplayTag>(new FGameplayTag());
			}
		}
		DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 14))
	{
		if (Ar.IsLoading())
		{
			if (!StatusEffectType.IsValid())
			{
				StatusEffectType = TSharedPtr<FGameplayTag>(new FGameplayTag());
			}
		}
		StatusEffectType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 15))
	{
		DeathImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 16))
	{
		AirborneForce.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 17))
	{
		Ar << bIsExecuteHit;
	}
	if (RepBits & (1 << 18))
	{
		Ar << bIsRadialDamage;
 		
		if (RepBits & (1 << 19))
		{
			Ar << RadialDamageInnerRadius;
		}
		if (RepBits & (1 << 20))
		{
			Ar << RadialDamageOuterRadius;
		}
		if (RepBits & (1 << 21))
		{
			RadialDamageOrigin.NetSerialize(Ar, Map, bOutSuccess);
		}
	}

	
	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}	

	bOutSuccess = true;
	return true;
}
