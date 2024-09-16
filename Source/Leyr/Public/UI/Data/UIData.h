#pragma once

#include "CoreMinimal.h"
#include "UIData.generated.h"

UENUM(BlueprintType, Blueprintable)
enum class EMessageType : uint8
{
	DamageToEnemy,
	DamageToPlayer,
	Experience,
	
};

USTRUCT(BlueprintType, Blueprintable)
struct FUIMessageData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) EMessageType MessageType = EMessageType::DamageToEnemy;
	UPROPERTY(BlueprintReadOnly) float Value = 0.f;
	UPROPERTY(BlueprintReadOnly) AActor* TargetActor = nullptr;
	UPROPERTY(BlueprintReadOnly) bool bBlockedHit = false;
	UPROPERTY(BlueprintReadOnly) bool bCriticalHit = false;
};
