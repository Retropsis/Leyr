// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/MovingPlatform.h"
#include "WeightingPlatform.generated.h"

class UPaperSprite;

UENUM()
enum class EMovementDirection : uint8
{
	Still,
	Downward,
	Upward,
};

/**
 * 
 */
UCLASS()
class LEYR_API AWeightingPlatform : public AMovingPlatform
{
	GENERATED_BODY()

public:
	AWeightingPlatform();
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	void MoveTo(const FVector& InCurrentTarget, float Speed, float InterpolationSpeed, float DeltaSeconds) const;
	void SetPlatformDirection(AActor* OtherActor, bool bDownward);

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(EditAnywhere, Category="Platform")
	bool bIsDamping = false;

	UPROPERTY(EditAnywhere, Category="Platform")
	float DownwardSpeed = 50.f;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	float DownwardInterpSpeed = 1.f;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	float UpwardSpeed = 50.f;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	float UpwardInterpSpeed = 1.f;

private:
	FVector UpPosition = FVector::ZeroVector;
	FVector DownPosition = FVector::ZeroVector;
	EMovementDirection MovementDirection = EMovementDirection::Still;
};
