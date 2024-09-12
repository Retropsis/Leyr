// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/PlatformInterface.h"
#include "World/Level/Hanger.h"
#include "Rope.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API ARope : public AHanger, public IPlatformInterface
{
	GENERATED_BODY()

public:
	ARope();

protected:
	virtual void BeginPlay() override;
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual void HandleIgnoreCollisionEnd() override;
};
