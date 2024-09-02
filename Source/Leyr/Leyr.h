// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Leyr.generated.h"

#define ECC_Player ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Enemy ECollisionChannel::ECC_GameTraceChannel2
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel3
#define ECC_OneWayPlatform ECollisionChannel::ECC_GameTraceChannel4
#define EOT_PlayerCapsule ObjectTypeQuery7
#define EOT_EnemyCapsule ObjectTypeQuery8
#define EOT_Projectile ObjectTypeQuery9
#define EOT_OneWayPlatform ObjectTypeQuery10
#define EOT_VaultDownPlatform ObjectTypeQuery11

USTRUCT(BlueprintType)
struct FValueRange
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Meta")
	FScalableFloat ValueMin = FScalableFloat();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Meta")
	FScalableFloat ValueMax = FScalableFloat();

	float GetRandomFloatFromScalableRange(const float Scaler) const
	{
		return FMath::FRandRange(ValueMin.GetValueAtLevel(Scaler), ValueMax.GetValueAtLevel(Scaler));
	}
};