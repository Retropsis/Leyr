// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/Moving/MovingGroup.h"
#include "WaterGroup.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API AWaterGroup : public AMovingGroup
{
	GENERATED_BODY()

public:
	AWaterGroup();
	virtual void HandleActiveActors(float DeltaSeconds) override;
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(EditAnywhere, Category="Moving|Entangled")
	float SwimmingSpeed = 50.f;

	UPROPERTY(EditAnywhere, Category="Moving|Entangled")
	float SwimmingGravityScale = .2f;

private:
	float SurfaceZ = 0.f;
};
