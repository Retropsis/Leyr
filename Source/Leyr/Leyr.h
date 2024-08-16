// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Leyr.generated.h"

#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1

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
		return FMath::FRandRange(ValueMin.GetValueAtLevel(Scaler), ValueMin.GetValueAtLevel(Scaler));
	}
};