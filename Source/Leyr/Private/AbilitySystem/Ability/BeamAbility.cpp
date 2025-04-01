// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/BeamAbility.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Game/BaseGameplayTags.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void UBeamAbility::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		HitLocation = HitResult.ImpactPoint;
		HitActor = HitResult.GetActor();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UBeamAbility::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

bool UBeamAbility::StoreFirstTarget()
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(OwnerCharacter);
		FHitResult HitResult;
		const FVector Start = ICombatInterface::Execute_GetCombatSocketLocation(OwnerCharacter, FBaseGameplayTags::Get().CombatSocket_Weapon);
		const FVector End = Start + OwnerCharacter->GetActorForwardVector() * LivePlayersWithinRadius;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		TEnumAsByte<EObjectTypeQuery> OT = GetAvatarActorFromActorInfo()->ActorHasTag(FName("Player")) ? EOT_EnemyCapsule : ObjectTypeQuery3;
		ObjectTypes.Add(OT);
		UKismetSystemLibrary::SphereTraceSingleForObjects(OwnerCharacter, Start, End, 10.f, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);

		if (HitResult.bBlockingHit)
		{
			HitLocation = HitResult.ImpactPoint;
			HitActor = HitResult.GetActor();
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(HitActor))
			{
				CombatInterface->GetOnDeath().AddUniqueDynamic(this, &UBeamAbility::PrimaryTargetDied);
			}
			return true;
		}
		HitLocation = HitResult.TraceEnd;
	}
	return false;
}

void UBeamAbility::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(OwnerCharacter);
		FHitResult HitResult;
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(OwnerCharacter, FBaseGameplayTags::Get().CombatSocket_Weapon);
		UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter, SocketLocation, BeamTargetLocation, 10.f, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

		if (HitResult.bBlockingHit)
		{
			HitLocation = HitResult.ImpactPoint;
			HitActor = HitResult.GetActor();
		}
		else
		{
			HitLocation = HitResult.TraceEnd;
		}
	}
}

void UBeamAbility::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(HitActor);
 	
	TArray<AActor*> OverlappingActors;
	ULeyrAbilitySystemLibrary::GetLivePlayersWithinRadius(GetAvatarActorFromActorInfo(), OverlappingActors, ActorsToIgnore, 850.f, HitActor->GetActorLocation());
 	
	//int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);
	int32 NumAdditionTargets = 5;
 	
	ULeyrAbilitySystemLibrary::GetClosestTargets(NumAdditionTargets, OverlappingActors, OutAdditionalTargets, HitActor->GetActorLocation());

	for (AActor* Actor : OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Actor))
		{
			CombatInterface->GetOnDeath().AddUniqueDynamic(this, &UBeamAbility::AdditionalTargetDied);
		}
	}
}

void UBeamAbility::ClearPrimaryOnDeathDelegate()
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(HitActor))
	{
		CombatInterface->GetOnDeath().RemoveDynamic(this, &ThisClass::PrimaryTargetDied);
	}
}

void UBeamAbility::ClearAdditionalOnDeathDelegate(AActor* Actor)
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Actor))
	{
		CombatInterface->GetOnDeath().RemoveDynamic(this, &ThisClass::AdditionalTargetDied);
	}
}