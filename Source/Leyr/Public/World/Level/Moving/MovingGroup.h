// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingGroup.generated.h"


class UBoxComponent;

UENUM()
enum class EMovingDirection : uint8
{
	Sink,
	Vector,
};

UCLASS()
class LEYR_API AMovingGroup : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingGroup();
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	virtual void HandleActiveActors(float DeltaSeconds);
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditDefaultsOnly, Category="Platform")
	TObjectPtr<UBoxComponent> OverlapBox;

	UPROPERTY(EditAnywhere, Category="Moving")
	float EntangledWalkSpeed = 50.f;

	UPROPERTY(EditAnywhere, Category="Moving")
	float EntangledGravityScale = .2f;
	
	UPROPERTY(EditAnywhere, Category="Moving")
	float InterpolationSpeed = .08f;
	
	UPROPERTY(EditAnywhere, Category="Moving")
	FVector Direction { 1.f, 0.f, 0.f };

	FVector InterpTarget = FVector::ZeroVector;
	float MinZ = 0.f;

private:
	UPROPERTY()
	TArray<AActor*> ActiveActors = TArray<AActor*>();
	
	bool bIsActive = false;
};
