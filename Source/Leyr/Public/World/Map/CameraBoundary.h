// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperTileMapActor.h"
#include "Components/BoxComponent.h"
#include "Data/EncounterSpawnData.h"
#include "Data/MapData.h"
#include "CameraBoundary.generated.h"

class UEntranceMarker;
class AEntrance;
enum class ERoomType : uint8;
enum class ERoomUpdateType : uint8;
struct FActiveGameplayEffectHandle;
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
	
	UFUNCTION(CallInEditor, Category=" Camera Boundary")
	virtual void InitializeCameraExtent();
	void CreateSpawningVolume(const FActorSpawnParameters& SpawnParams, const FEncounterSpawn& Data, const FVector& Offset, const FString& Label);

	UFUNCTION(CallInEditor, Category=" Camera Boundary")
	virtual void InitializeSpawnVolumes();
	
	UFUNCTION(CallInEditor, Category=" Camera Boundary")
	virtual void UpdateSpawnVolumes();
	
	UFUNCTION(CallInEditor, Category=" Camera Boundary")
	virtual void ClearSpawnVolumes();
	
	UFUNCTION(CallInEditor, Category=" Camera Boundary")
	void SpawnWaterVolume();
	
	UFUNCTION(CallInEditor, Category=" Camera Boundary")
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
	FIntPoint GetRoomCoordinates() const;
	FName GetTileMapName() const;
	FName GetLevelAreaName() const { return LevelAreaName; }
	ERoomType GetRoomType() const { return RoomType; } 
	
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
	
	UPROPERTY(EditAnywhere, Category=" Camera Boundary")
	TObjectPtr<ULevelAreaData> LevelAreaData;
	
	UPROPERTY(EditAnywhere, Category=" Camera Boundary")
	TObjectPtr<APaperTileMapActor> TileMap;
	
	UPROPERTY(EditDefaultsOnly, Category=" Camera Boundary")
	TSubclassOf<UEntranceMarker> EntranceMarkerClass;

	UPROPERTY(EditAnywhere, Category=" Camera Boundary")
	TArray<TObjectPtr<UEntranceMarker>> EntranceMarkers;
	
	UPROPERTY(EditAnywhere, Category=" Camera Boundary")
	EBoundaryRule BoundaryRule = EBoundaryRule::Extent;

	UPROPERTY(EditAnywhere, Category=" Camera Boundary")
	bool bConstrainZ = true;
	
	UPROPERTY(EditAnywhere, Category=" Camera Boundary")
	bool bConstrainX = true;

	/*
	 * Other Game Mechanics
	 */
	UPROPERTY(EditDefaultsOnly, Category=" Camera Boundary")
	TSubclassOf<AEncounterSpawnVolume> SpawningVolumeClass;
	
	UPROPERTY(VisibleAnywhere, Category=" Camera Boundary")
	TArray<TObjectPtr<AEncounterSpawnVolume>> SpawningVolumes;

	UPROPERTY(EditDefaultsOnly, Category=" Camera Boundary")
	TSubclassOf<AWaterGroup> WaterVolumeClass;
	
	UPROPERTY(VisibleAnywhere, Category="Camera Boundary")
	TObjectPtr<AWaterGroup> WaterVolume;

	UPROPERTY()
	TArray<TScriptInterface<ILevelActorInterface>> LevelActors;
		
	UPROPERTY(EditAnywhere, Category=" Camera Boundary|Gameplay Effects", meta=(TitleProperty=GameplayEffect))
	TArray<FLevelArea_GameplayEffect> GrantedGameplayEffects;
	
	UPROPERTY()
	TArray<TObjectPtr<UNiagaraSystem>> EnvironmentEffects;
	
	UPROPERTY()
	TArray<UNiagaraComponent*> SpawnedEnvironmentEffects;

private:
	void RequestRoomUpdate(ERoomUpdateType UpdateType) const;
	FIntPoint GetPlayerRoomCoordinates() const;
	APlayerController* GetPlayerController();
	
	void AddEntrance(int32 Index);
	void RemoveEntrance(int32 Index);
	void RemoveAllEntrances();
	void SwapEntrances(int32 Index);
	
	UPROPERTY(VisibleAnywhere, Category="Camera Boundary") ERoomType RoomType;
	UPROPERTY(VisibleAnywhere, Category="Camera Boundary") FName LevelAreaName = FName();;

	UPROPERTY() TObjectPtr<APlayerController> PlayerController;
	UPROPERTY() TObjectPtr<AActor> TargetActor;
	UPROPERTY() TMap<FIntPoint, FSubdivision> Subdivisions;
	UPROPERTY(VisibleAnywhere, Category="Camera Boundary") TArray<TObjectPtr<UEntranceMarker>> PreEditEntranceMarkers;
	UPROPERTY() FLevelArea_GrantedHandles LevelArea_GrantedHandles;
	UPROPERTY() FBoxSphereBounds TileMapBounds;
	UPROPERTY() bool bEnvironmentEffectsInitialized = false;
};
