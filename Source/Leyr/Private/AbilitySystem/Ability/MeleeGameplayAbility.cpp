// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/MeleeGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/InteractionInterface.h"
#include "Kismet/KismetSystemLibrary.h"

FHitResult UMeleeGameplayAbility::BoxTrace()
{
	ICombatInterface::Execute_GetAttackAnimationData(GetAvatarActorFromActorInfo(), BoxTraceStart, BoxTraceEnd);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte<EObjectTypeQuery> OT = GetAvatarActorFromActorInfo()->ActorHasTag(FName("Player")) ? EOT_EnemyCapsule : ObjectTypeQuery3;
	ObjectTypes.Add(OT);
	// ObjectTypes.Add(ICombatInterface::Execute_GetTraceObjectType(GetAvatarActorFromActorInfo()));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	FHitResult Hit;
	UKismetSystemLibrary::BoxTraceSingleForObjects(
		this, BoxTraceStart, BoxTraceEnd, BoxTraceExtent, FRotator::ZeroRotator, ObjectTypes,
		false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true);

	TArray<FHitResult> Hits;
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		this, BoxTraceStart, BoxTraceEnd, BoxTraceExtent, FRotator::ZeroRotator, ObjectTypes,
		false, ActorsToIgnore, EDrawDebugTrace::None, Hits, true);
	
	for (FHitResult HitResult : Hits)
	{
		if(HitResult.bBlockingHit && HitResult.GetActor() && HitResult.GetActor()->ActorHasTag("HitInteraction"))
		{
			IInteractionInterface::Execute_Interact(HitResult.GetActor());
		}
	}

	return Hit;
}
