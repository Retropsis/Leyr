#pragma once

#include "CoreMinimal.h"
#include "EquipmentData.generated.h"

UENUM(BlueprintType)
enum class EEquipmentAnimationState : uint8
{
	Default UMETA(DisplayName="Default"),
	OneHanded UMETA(DisplayName="OneHanded"),
	TowHanded UMETA(DisplayName="TowHanded"),
	Dagger UMETA(DisplayName="Dagger"),
	Staff UMETA(DisplayName="Staff"),
	Spear UMETA(DisplayName="Spear"),
	Swimming UMETA(DisplayName="Swimming"),
};

USTRUCT(BlueprintType)
struct FEquipmentData
{
	GENERATED_BODY()

	FName SocketName = FName("RightHandSocket");
	EEquipmentAnimationState EquipmentAnimationState = EEquipmentAnimationState::Default;
};
