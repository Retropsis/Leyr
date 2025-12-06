// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperTileMapActor.h"
#include "Components/BoxComponent.h"
#include "Data/EncounterSpawnData.h"
#include "CameraBoundary.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
struct FActiveGameplayEffectHandle;
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
	
	UFUNCTION(CallInEditor, Category=" Camera Boundary")
	virtual void InitializeCameraExtent();
	void CreateSpawningVolume(const FActorSpawnParameters& SpawnParams, const FEncounterSpawn& Data, const FVector& Offset, const FString& Label);

	UFUNCTION(CallInEditor, Category=" Camera Boundary")
	virtual void InitializeSpawnVolumes();
	
	UFUNCTION(CallInEditor, Category=" Camera Boundary")
	virtual void UpdateSpawnVolumes();
	
	UFUNCTION(CallInEditor, Category=" Camera Boundary")
	virtual void ClearSpawnVolumes();
	
	AActor* GetTargetActor() { return TargetActor; }
	void SetTargetActor(AActor* InTargetActor) { TargetActor = InTargetActor; }
	virtual void HandleOnBeginOverlap(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	FBoxSphereBounds GetBounds() const { return CameraBoundary->Bounds; }
	
	UFUNCTION(BlueprintCallable)
	FBoxSphereBounds GetEnteringBounds() const { return EnteringBoundary->Bounds; }
	
	UFUNCTION(BlueprintCallable)
	FBoxSphereBounds GetNavigationBounds() const { return NavigationBoundary->Bounds; }
	
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
	
	UPROPERTY(EditAnywhere, Category=" Camera Boundary")
	EBoundaryRule BoundaryRule = EBoundaryRule::Extent;

	UPROPERTY(EditAnywhere, Category=" Camera Boundary")
	bool bConstrainZ = true;
	
	UPROPERTY(EditAnywhere, Category=" Camera Boundary")
	bool bConstrainX = true;

	/*
	 * Other Game Mechanics
	 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEncounterSpawnVolume> SpawningVolumeClass;
	
	UPROPERTY(VisibleAnywhere, Category=" Camera Boundary")
	TArray<TObjectPtr<AEncounterSpawnVolume>> SpawningVolumes;

	UPROPERTY()
	TArray<TScriptInterface<ILevelActorInterface>> LevelActors;
		
	UPROPERTY(EditAnywhere, Category=" Camera Boundary|Gameplay Effects", meta=(TitleProperty=GameplayEffect))
	TArray<FLevelArea_GameplayEffect> GrantedGameplayEffects;
	
	UPROPERTY()
	TArray<TObjectPtr<UNiagaraSystem>> EnvironmentEffects;
	
	UPROPERTY()
	TArray<UNiagaraComponent*> SpawnedEnvironmentEffects;

private:
	UPROPERTY() TObjectPtr<AActor> TargetActor;
	UPROPERTY() FLevelArea_GrantedHandles LevelArea_GrantedHandles;
	FBoxSphereBounds TileMapBounds;
	bool bEnvironmentEffectsInitialized = false;
};
