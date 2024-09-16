// @ Retropsis 2024-2025.

#include "AbilitySystem/Data/ActorClassInfo.h"

FActorClassDefaultInfo UActorClassInfo::GetClassDefaultInfo(EActorClass ActorClass)
{
	return ActorClassInformation.FindChecked(ActorClass);
}
