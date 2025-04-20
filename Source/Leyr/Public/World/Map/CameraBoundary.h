// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperTileMapActor.h"
#include "Components/BoxComponent.h"
#include "CameraBoundary.generated.h"

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

	UFUNCTION(CallInEditor, Category="00 - Camera Boundary")
	virtual void InitializeCameraExtent();
	
	UFUNCTION(CallInEditor, Category="00 - Camera Boundary")
	virtual void InitializeSpawnVolumes();
	
	UFUNCTION(CallInEditor, Category="00 - Camera Boundary")
	virtual void ClearSpawnVolumes();
	
	AActor* GetTargetActor() { return TargetActor; }
	void SetTargetActor(AActor* InTargetActor) { TargetActor = InTargetActor; }
	virtual void HandleOnBeginOverlap(AActor* OtherActor);
	FBoxSphereBounds GetBounds() const { return CameraBoundary->Bounds; }
	FBoxSphereBounds GetEnteringBounds() const { return EnteringBoundary->Bounds; }
	FBoxSphereBounds GetNavigationBounds() const { return NavigationBoundary->Bounds; }
	
	FOnPlayerLeaving OnPlayerLeaving;
	FOnPlayerEntering OnPlayerEntering;

protected:
	virtual void BeginPlay() override;
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
	
	UPROPERTY(EditAnywhere, Category="00 - Camera Boundary")
	TObjectPtr<ULevelAreaData> LevelAreaData;
	
	UPROPERTY(EditAnywhere, Category="00 - Camera Boundary")
	TObjectPtr<APaperTileMapActor> TileMap;
	
	UPROPERTY(EditAnywhere, Category="00 - Camera Boundary")
	EBoundaryRule BoundaryRule = EBoundaryRule::Extent;

	UPROPERTY(EditAnywhere, Category="00 - Camera Boundary")
	bool bConstrainZ = true;
	
	UPROPERTY(EditAnywhere, Category="00 - Camera Boundary")
	bool bConstrainX = true;

	/*
	 * Other Game Mechanics
	 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEncounterSpawnVolume> SpawningVolumeClass;
	
	UPROPERTY()
	TArray<TObjectPtr<AEncounterSpawnVolume>> SpawningVolumes;

	UPROPERTY()
	TArray<TScriptInterface<ILevelActorInterface>> LevelActors;
		
	UPROPERTY(EditAnywhere, Category="00 - Camera Boundary|Gameplay Effects", meta=(TitleProperty=GameplayEffect))
	TArray<FLevelArea_GameplayEffect> GrantedGameplayEffects;

private:
	UPROPERTY() TObjectPtr<AActor> TargetActor;
	UPROPERTY() FLevelArea_GrantedHandles LevelArea_GrantedHandles;
};
