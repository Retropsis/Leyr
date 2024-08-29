// @ Retropsis 2024-2025.

#include "Equipment/Equippable.h"

FEquipmentData AEquippable::GetEquipmentData_Implementation()
{
	return FEquipmentData{ SocketName, EquipmentAnimationState };
}
