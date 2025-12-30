// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentData.h"
#include "Interaction/EquipmentInterface.h"
#include "World/Item/Item.h"
#include "Equippable.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API AEquippable : public AItem, public IEquipmentInterface
{
	GENERATED_BODY()

public:
	//~ Equipment Interface
	virtual FEquipmentData GetEquipmentData_Implementation() override;
	//~ Equipment Interface

	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	FName SocketName = FName("");

	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	EEquipmentAnimationState EquipmentAnimationState = EEquipmentAnimationState::Default;
};
