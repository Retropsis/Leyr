// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/Hanger.h"
#include "ClimbingSurface.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API AClimbingSurface : public AHanger
{
	GENERATED_BODY()

public:
	AClimbingSurface();

	//~ Interact Interface
	virtual void Interact_Implementation(AActor* InteractingActor) override;
	//~ Interact Interface

protected:
	virtual void BeginPlay() override;
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	virtual void HandleIgnoreCollisionEnd() override;
	
};
