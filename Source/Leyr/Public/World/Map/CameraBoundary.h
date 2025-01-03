// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World/Data/CameraData.h"
#include "CameraBoundary.generated.h"

class USpringArmComponent;
class UTimelineComponent;
class UBoxComponent;
enum class EExitDirection : uint8;

UCLASS()
class LEYR_API ACameraBoundary : public AActor
{
	GENERATED_BODY()
	
public:	
	ACameraBoundary();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void ResetSocket();

	UFUNCTION(BlueprintCallable)
	void InterpSocketOffset(float Alpha);

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Boundary;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BoundaryVisualizer;

	TObjectPtr<UTimelineComponent> TimelineComponent;

	UPROPERTY(EditAnywhere)
	EExitDirection ExitDirection = EExitDirection::Horizontal;

private:
	void SetSocket();
	float VerticalSocketOffsetZ() const;
	float HorizontalSocketOffsetY() const;
	
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY() TObjectPtr<AActor> Player;
	FVector ExitLocation = FVector::ZeroVector;
	FVector OriginalSocketOffset = FVector::ZeroVector;
	FVector ResetSocketOffset = FVector::ZeroVector;
};
