// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "MapData.h"
#include "Engine/DataAsset.h"
#include "LevelAreaData.generated.h"

struct FGameplayTag;
struct FEncounterSpawn;
class UNiagaraSystem;
class UGameplayEffect;
class UEncounterSpawnData;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelAreaDataPropertyChanged, FName /* PropertyName */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEncounterSpawnsPropertyChanged, FName /* PropertyName */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEncounterSpawnsPropertyDeleted, FGameplayTag /* EncounterSpawnTag */);

UCLASS(BlueprintType)
class LEYR_API ULevelAreaData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:	
#if WITH_EDITOR
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

	FOnLevelAreaDataPropertyChanged OnLevelAreaDataPropertyChanged;
	FOnEncounterSpawnsPropertyChanged OnEncounterSpawnsPropertyChanged;
	FOnEncounterSpawnsPropertyDeleted OnEncounterSpawnsPropertyDeleted;
	
	UPROPERTY(EditDefaultsOnly)
	FName LevelAreaName = FName("");
	
	UPROPERTY(EditDefaultsOnly)
	ERoomType RoomType = ERoomType::None;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FEncounterSpawn> EncounterSpawns;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> GrantedGameplayEffects;

	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UNiagaraSystem>> EnvironmentEffects;
	
	TArray<FEncounterSpawn> TempEncounterSpawns;
};
