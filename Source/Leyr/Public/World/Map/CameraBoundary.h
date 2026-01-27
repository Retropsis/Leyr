// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperTileMapActor.h"
#include "Components/BoxComponent.h"
#include "Data/MapData.h"
#include "CameraBoundary.generated.h"

struct FActiveGameplayEffectHandle;
class UEntranceMarker;
class AWaterGroup;
class UNiagaraComponent;
class UNiagaraSystem;
class UGameplayEffect;
class UAbilitySystemComponent;
class ULevelAreaData;
class ILevelActorInterface;
class AEncounterSpawnVolume;
class UBoxComponent;

/**
 * FLevelArea_GameplayEffect
 *
 *	Data used by the Level Area to grant gameplay effects.
 */
USTRUCT(BlueprintType)
struct FLevelArea_GameplayEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	UPROPERTY(EditAnywhere)
	float Level = 1.0f;
};

/**
 * FLevelArea_GrantedHandles
 *
 *	Data used to store handles to what has been granted by the Level Area.
 */
USTRUCT(BlueprintType)
struct FLevelArea_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void TakeFromAbilitySystem(UAbilitySystemComponent* ASC);

protected:
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;
};

DECLARE_MULTICAST_DELEGATE(FOnPlayerLeaving);
DECLARE_MULTICAST_DELEGATE(FOnPlayerEntering);

UENUM(BlueprintType)
enum class EBoundaryRule : uint8
{
	Detachment,
	Extent,
	Arena,
};

UCLASS()
class LEYR_API ACameraBoundary : public AActor
{
	GENERATED_BODY()
	
public:	
	ACameraBoundary();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
	
	UFUNCTION(CallInEditor, Category="Leyr")
	virtual void GroupIntoRoomFolder();
	
	UFUNCTION(CallInEditor, Category="Leyr")
	virtual void RenameVolumes();
	
	UFUNCTION(CallInEditor, Category="Leyr")
	virtual void InitializeCameraExtent();
	
	UFUNCTION(CallInEditor, Category="Leyr")
	void SpawnWaterVolume();
	
	UFUNCTION(CallInEditor, Category="Leyr")
	void UpdateWaterVolume() const;
	
	UFUNCTION(CallInEditor, Category="Leyr")
	void ClearWaterVolume();
	
	AActor* GetTargetActor() { return TargetActor; }
	void SetTargetActor(AActor* InTargetActor) { TargetActor = InTargetActor; }
	virtual void HandleOnBeginOverlap(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	FBoxSphereBounds GetBounds() const { return CameraBoundary->Bounds; }
	
	UFUNCTION(BlueprintCallable)
	FBoxSphereBounds GetEnteringBounds() const { return EnteringBoundary->Bounds; }
	
	UFUNCTION(BlueprintCallable)
	FBoxSphereBounds GetNavigationBounds() const { return NavigationBoundary->Bounds; }

	TMap<FIntPoint, FSubdivision> ConstructSubdivisions();
	FIntPoint GetRoomSize() const;
	FName GetTileMapName() const;
	FVector GetTileMapLocation() const;
	FName GetLevelAreaName() const { return LevelAreaName; }
	ERoomType GetRoomType() const { return RoomType; }

	UFUNCTION()
	void OnHiddenRevealed(const FIntPoint& SubdivisionCoordinates, ESubdivisionSide Side);
	
	FOnPlayerLeaving OnPlayerLeaving;
	FOnPlayerEntering OnPlayerEntering;

protected:
	virtual void BeginPlay() override;
	FBoxSphereBounds GetTileMapBounds() const;
	void DestroyOutOfBoundsEncounters() const;
	void ToggleLevelActorActivity(bool bActivate) const;
	void GiveToAbilitySystem(UAbilitySystemComponent* ASC, FLevelArea_GrantedHandles* OutGrantedHandles, float Level = 1.f, UObject* SourceObject = nullptr) const;
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> EnteringBoundary;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> NavigationBoundary;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CameraBoundary;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BoundaryVisualizer;
	
	UPROPERTY(EditAnywhere, Category="Leyr")
	TObjectPtr<ULevelAreaData> LevelAreaData;
	
	UPROPERTY(VisibleAnywhere, Category="Leyr")
	FName LevelAreaName = FName();
	
	UPROPERTY(EditAnywhere, Category="Leyr")
	TArray<TObjectPtr<UEntranceMarker>> EntranceMarkers;
	
	UPROPERTY(VisibleAnywhere, Category="Leyr")
	TArray<TObjectPtr<UEntranceMarker>> PreEditEntranceMarkers;
	
	UPROPERTY(EditAnywhere, Category="Leyr")
	TObjectPtr<APaperTileMapActor> TileMap;
	
	UPROPERTY(EditDefaultsOnly, Category="Leyr")
	TSubclassOf<UEntranceMarker> EntranceMarkerClass;
	
	UPROPERTY(EditAnywhere, Category="Leyr")
	EBoundaryRule BoundaryRule = EBoundaryRule::Extent;

	UPROPERTY(EditAnywhere, Category="Leyr")
	bool bConstrainZ = true;
	
	UPROPERTY(EditAnywhere, Category="Leyr")
	bool bConstrainX = true;
	
	UPROPERTY(VisibleAnywhere, Category="Leyr")
	ERoomType RoomType;

	/*
	 * Other Game Mechanics
	 */
	UPROPERTY(EditDefaultsOnly, Category="Leyr")
	TSubclassOf<AEncounterSpawnVolume> SpawningVolumeClass;
	
	UPROPERTY(EditAnywhere, Category="Leyr")
	TArray<TObjectPtr<AEncounterSpawnVolume>> SpawningVolumes;
	
	UPROPERTY(VisibleAnywhere, Category="Leyr")
	TArray<TObjectPtr<AEncounterSpawnVolume>> PreEditSpawningVolumes;

	UPROPERTY(EditDefaultsOnly, Category="Leyr|Water")
	TSubclassOf<AWaterGroup> WaterVolumeClass;
	
	UPROPERTY(VisibleAnywhere, Category="Leyr|Water")
	TObjectPtr<AWaterGroup> WaterVolume;

	UPROPERTY()
	TArray<TScriptInterface<ILevelActorInterface>> LevelActors;
		
	UPROPERTY(EditAnywhere, Category="Leyr", meta=(TitleProperty=GameplayEffect))
	TArray<FLevelArea_GameplayEffect> GrantedGameplayEffects;
	
	UPROPERTY()
	TArray<TObjectPtr<UNiagaraSystem>> EnvironmentEffects;
	
	UPROPERTY()
	TArray<UNiagaraComponent*> SpawnedEnvironmentEffects;

private:
	void RequestRoomUpdate(ERoomUpdateType UpdateType) const;
	APlayerController* GetPlayerController();
	FName GetValidRoomName() const;
	FName GetValidRoomNameTrimmed() const;
	FName MakeFolderPathFromRoomName() const;
	
	void AddEntrance(int32 Index);
	void RemoveEntrance(int32 Index);
	void RemoveAllEntrances();

	void AddSpawnVolume(int32 Index);
	void RemoveSpawnVolume(int32 Index);
	void RemoveAllSpawnVolumes();

	void GetAllOverlappingActors(TArray<FOverlapResult>& Overlaps) const;

	UPROPERTY() TObjectPtr<APlayerController> PlayerController;
	UPROPERTY() TObjectPtr<AActor> TargetActor;
	UPROPERTY() TMap<FIntPoint, FSubdivision> Subdivisions;
	UPROPERTY() FLevelArea_GrantedHandles LevelArea_GrantedHandles;
	UPROPERTY() FBoxSphereBounds TileMapBounds;
	UPROPERTY() bool bEnvironmentEffectsInitialized = false;
};
