#pragma once

#include "CoreMinimal.h"
#include "AbilityDescriptionData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FAbilityDescription
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	bool bShowLevel = true;
	
	UPROPERTY(EditDefaultsOnly)
	bool bShowMinValue = false;
	
	UPROPERTY(EditDefaultsOnly)
	bool bShowMaxValue = false;
	
	UPROPERTY(EditDefaultsOnly)
	bool bShowCooldown = false;
	
	UPROPERTY(EditDefaultsOnly)
	bool bShowManaCost = false;
	
	UPROPERTY(EditDefaultsOnly)
	FString Name = FString();

	UPROPERTY(EditDefaultsOnly)
	float CustomValue = 0.f;
	
	UPROPERTY(EditDefaultsOnly)
	FString CustomValueText = FString();
};