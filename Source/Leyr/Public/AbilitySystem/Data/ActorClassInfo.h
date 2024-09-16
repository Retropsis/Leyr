// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "ActorClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EActorClass : uint8
{
	Default,
	ApplyDamage,
	TakeDamage,
};

USTRUCT(BlueprintType)
struct FActorClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> Attributes;
	
	// UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	// TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	FScalableFloat XPReward = FScalableFloat();
};

/**
 * 
 */
UCLASS()
class LEYR_API UActorClassInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Actor Class Defaults")
	TMap<EActorClass, FActorClassDefaultInfo>ActorClassInformation;
	
	// UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	// TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	FActorClassDefaultInfo GetClassDefaultInfo(EActorClass ActorClass);
	
};
