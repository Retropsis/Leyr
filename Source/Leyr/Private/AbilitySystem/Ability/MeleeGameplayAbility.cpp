// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/MeleeGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

FHitResult UMeleeGameplayAbility::BoxTrace()
{
	FVector Start =  FVector::ZeroVector;
	FVector End = FVector::ZeroVector;
	ICombatInterface::Execute_GetAttackAnimationData(GetAvatarActorFromActorInfo(), Start, End);
	const FVector BoxExtent = FVector(40.f, 20.f, 30.f);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte<EObjectTypeQuery> OT = GetAvatarActorFromActorInfo()->ActorHasTag(FName("Player")) ? EOT_EnemyCapsule : ObjectTypeQuery3;
	ObjectTypes.Add(OT);
	// ObjectTypes.Add(ICombatInterface::Execute_GetTraceObjectType(GetAvatarActorFromActorInfo()));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	FHitResult Hit;
	UKismetSystemLibrary::BoxTraceSingleForObjects(
		this, Start, End, BoxExtent, FRotator::ZeroRotator, ObjectTypes,
		false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true);

	return Hit;

	// if(Hit.bBlockingHit && Hit.GetActor())
	// {
	// 	if (GetAvatarActorFromActorInfo()->HasAuthority())
	// 	{
	// 		CauseDamage(Hit.GetActor());
	// 	}
	// }
}
