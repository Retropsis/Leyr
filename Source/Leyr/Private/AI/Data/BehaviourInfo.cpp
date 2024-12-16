// @ Retropsis 2024-2025.


#include "AI/Data/BehaviourInfo.h"

FBehaviourDefaultInfo UBehaviourInfo::GetBehaviourDefaultInfo(FName BehaviourName)
{
	return BehaviourInformation.FindChecked(BehaviourName);
}
