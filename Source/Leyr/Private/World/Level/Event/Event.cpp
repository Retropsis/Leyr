// @ Retropsis 2024-2025.

#include "World/Level/Event/Event.h"

AEvent::AEvent()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEvent::SetEventState(const EEventState NewState)
{
	EventState = NewState;
	OnEventStateChanged.Broadcast(EventState);
}

