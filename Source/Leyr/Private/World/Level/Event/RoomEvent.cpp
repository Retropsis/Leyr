// @ Retropsis 2024-2025.

#include "World/Level/Event/RoomEvent.h"
#include "Interaction/CombatInterface.h"

ARoomEvent::ARoomEvent()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARoomEvent::BeginPlay()
{
	Super::BeginPlay();

	OnEventStateChanged.Broadcast(EventState);
	
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(EventOwner))
	{
		CombatInterface->GetOnDeath().AddDynamic(this, &ThisClass::HandleEventOwnerDefeated);
	}
}

void ARoomEvent::LoadActor_Implementation()
{
	if (EventState == EEventState::On)
	{
		OnEventStateChanged.Broadcast(EventState);
	}
}

void ARoomEvent::HandleEventOwnerDefeated(AActor* DefeatedEncounter)
{
	EventState = EEventState::On;
	OnEventStateChanged.Broadcast(EventState);
}

