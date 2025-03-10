// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World/Data/CameraData.h"
#include "PaperTileMapActor.h"
#include "Components/BoxComponent.h"
#include "CameraBoundary.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTimelineComponent;
class UBoxComponent;
enum class EExitDirection : uint8;

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
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(CallInEditor, Category="Camera Boundary")
	virtual void InitializeCameraExtent();
	
	virtual void HandleOnBeginOverlap(AActor* OtherActor);
	virtual void HandleOnEndOverlap(AActor* OtherActor);

	void SetTargetActor(AActor* InTargetActor) { TargetActor = InTargetActor; }
	AActor* GetTargetActor() { return TargetActor; }
	FBoxSphereBounds GetBounds() const { return CameraBoundary->Bounds; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void ResetSocket();

	UFUNCTION(BlueprintCallable)
	void InterpSocketOffset(float InAlpha);

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> EnteringBoundary;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CameraBoundary;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BoundaryVisualizer;

	TObjectPtr<UTimelineComponent> TimelineComponent;
	
	UPROPERTY(EditAnywhere, Category="Camera Boundary")
	TObjectPtr<APaperTileMapActor> TileMap;

	UPROPERTY(EditAnywhere, Category="Camera Boundary")
	bool bConstrainZ = false;
	
	UPROPERTY(EditAnywhere, Category="Camera Boundary")
	bool bConstrainX = false;

	UPROPERTY(EditAnywhere, Category="Camera Boundary")
	EExitDirection ExitDirection = EExitDirection::Horizontal;
	
	UPROPERTY(EditAnywhere, Category="Camera Boundary")
	EBoundaryRule BoundaryRule = EBoundaryRule::Detachment;

private:
	void SetSocket();
	float VerticalSocketOffsetZ() const;
	float HorizontalSocketOffsetY() const;
	
	TObjectPtr<USpringArmComponent> SpringArm;
	TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY() TObjectPtr<AActor> Player;
	UPROPERTY() TObjectPtr<AActor> TargetActor;
	FVector ExitLocation = FVector::ZeroVector;
	FVector OriginalSocketOffset = FVector::ZeroVector;
	FVector ResetSocketOffset = FVector::ZeroVector;
	bool bShouldInterpZ = false;
	float Alpha = 0.f;
};
