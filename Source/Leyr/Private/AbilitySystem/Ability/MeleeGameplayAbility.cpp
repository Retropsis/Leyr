// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/MeleeGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/InteractionInterface.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FHitResult> UMeleeGameplayAbility::BoxTrace(bool bDebug)
{
	BoxTraceData = ICombatInterface::Execute_GetBoxTraceDataByTag(GetAvatarActorFromActorInfo(), MontageTag);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte<EObjectTypeQuery> OT = GetAvatarActorFromActorInfo()->ActorHasTag(FName("Player")) ? EOT_EnemyCapsule : ObjectTypeQuery3;
	ObjectTypes.Add(OT);
	// ObjectTypes.Add(ICombatInterface::Execute_GetTraceObjectType(GetAvatarActorFromActorInfo()));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	TArray<FHitResult> Hits;
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		this, BoxTraceData.Start, BoxTraceData.End, BoxTraceData.Extent, FRotator::ZeroRotator, ObjectTypes,
		false, ActorsToIgnore, bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, Hits, true);
	
	for (FHitResult HitResult : Hits)
	{
		if(HitResult.bBlockingHit && HitResult.GetActor() && HitResult.GetActor()->ActorHasTag("HitInteraction"))
		{
			IInteractionInterface::Execute_Interact(HitResult.GetActor(), GetAvatarActorFromActorInfo());
		}
	}
	return Hits;
}