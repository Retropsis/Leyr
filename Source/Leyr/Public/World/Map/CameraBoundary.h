// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperTileMapActor.h"
#include "Components/BoxComponent.h"
#include "CameraBoundary.generated.h"

class UBoxComponent;

DECLARE_MULTICAST_DELEGATE(FOnPlayerLeaving);

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

	UFUNCTION(CallInEditor, Category="Camera Boundary")
	virtual void InitializeCameraExtent();
	
	AActor* GetTargetActor() { return TargetActor; }
	void SetTargetActor(AActor* InTargetActor) { TargetActor = InTargetActor; }
	virtual void HandleOnBeginOverlap(AActor* OtherActor);
	FBoxSphereBounds GetBounds() const { return CameraBoundary->Bounds; }
	FBoxSphereBounds GetEnteringBounds() const { return EnteringBoundary->Bounds; }
	FBoxSphereBounds GetNavigationBounds() const { return NavigationBoundary->Bounds; }
	
	FOnPlayerLeaving OnPlayerLeaving;

protected:
	virtual void BeginPlay() override;

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
	
	UPROPERTY(EditAnywhere, Category="Camera Boundary")
	TObjectPtr<APaperTileMapActor> TileMap;
	
	UPROPERTY(EditAnywhere, Category="Camera Boundary")
	EBoundaryRule BoundaryRule = EBoundaryRule::Detachment;

	UPROPERTY(EditAnywhere, Category="Camera Boundary")
	bool bConstrainZ = false;
	
	UPROPERTY(EditAnywhere, Category="Camera Boundary")
	bool bConstrainX = false;

private:
	UPROPERTY() TObjectPtr<AActor> TargetActor;
};
