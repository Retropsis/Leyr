// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/Platform.h"
#include "PendulumPlatform.generated.h"

UENUM()
enum class EClockDirection : uint8
{
	Clockwise,
	CounterClockwise,
};

/**
 * 
 */
UCLASS()
class LEYR_API APendulumPlatform : public APlatform
{
	GENERATED_BODY()

public:
	APendulumPlatform();
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	TObjectPtr<USceneComponent> Center;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	TObjectPtr<USceneComponent> FirstPosition;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	TObjectPtr<USceneComponent> ApexPosition;

	UPROPERTY(BlueprintReadOnly)
	float Angle = 0.f;

private:
	EClockDirection ClockDirection = EClockDirection::Clockwise;
	FVector Direction = FVector::ZeroVector;
	float SpeedX = 1.f;
	float SpeedY = 1.f;
	float RadiusX = 100.f;
	float RadiusY = 100.f;
	float MinAngle = 0.f;
	float MaxAngle = 0.f;
};
