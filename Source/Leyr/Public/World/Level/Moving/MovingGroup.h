// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/LevelActorInterface.h"
#include "MovingGroup.generated.h"


class UPaperSpriteComponent;
class UBoxComponent;

UENUM()
enum class EMovingDirection : uint8
{
	Still,
	Sink,
	Vector,
};

UCLASS()
class LEYR_API AMovingGroup : public AActor, public ILevelActorInterface
{
	GENERATED_BODY()
	
public:	
	AMovingGroup();
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void Tick(float DeltaSeconds) override;
	void SetupWaterVolumeFromBounds(const FBoxSphereBounds& Bounds);
	void SetupWaterVolume(const int32 NewWidth, const int32 NewDepth);
	FBoxSphereBounds GetNavigationBounds() const;

	// LevelActorInterface~
	virtual void ResetState_Implementation() override {}
	virtual void ToggleActivate_Implementation(bool bActivate) override;
	// LevelActorInterface~

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void HandleActiveActors(float DeltaSeconds);
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, Category="Moving")
	TObjectPtr<UBoxComponent> OverlapBox;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPaperSpriteComponent> Sprite;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Moving")
	int32 Width = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Moving")
	int32 Depth = 3;
	
	UPROPERTY(EditAnywhere, Category="Moving|Entangled")
	float EntangledWalkSpeed = 60.f;
	
	UPROPERTY(EditAnywhere, Category="Moving|Entangled")
	float EntangledInterpSpeed = 50.f;

	UPROPERTY(EditAnywhere, Category="Moving|Entangled")
	float EntangledGravityScale = .2f;
	
	UPROPERTY(EditAnywhere, Category="Moving|Entangled")
	float InterpolationSpeed = 20.f;
	
	UPROPERTY(EditAnywhere, Category="Moving|Entangled")
	float FlowingInterpSpeed = 1.f;
	
	UPROPERTY(EditAnywhere, Category="Moving")
	FVector Direction { 1.f, 0.f, 0.f };

	UPROPERTY(EditAnywhere, Category="Moving")
	EMovingDirection MovingDirection = EMovingDirection::Sink;

	FVector InterpTarget = FVector::ZeroVector;
	float InterpTargetX = 0.f;
	float MinZ = 0.f;
	float SurfaceZ = 0.f;
	
	UPROPERTY()
	TArray<AActor*> ActiveActors = TArray<AActor*>();

private:
	bool bIsActive = false;
};
