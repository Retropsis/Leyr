// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interaction/ElevatorInterface.h"
#include "Elevator.generated.h"

class UBoxComponent;

UCLASS()
class LEYR_API AElevator : public APawn, public IElevatorInterface
{
	GENERATED_BODY()

public:
	AElevator();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Move_Implementation(float ScaleValue) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> FloorCollision;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> CeilingCollision;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> OccupancyCollision;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> TopPosition;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> BottomPosition;

	UPROPERTY(EditAnywhere)
	float ElevatorSpeed = 150.f;

private:
	float TopPositionZ = 0.f;
	float BottomPositionZ = 0.f;
};
