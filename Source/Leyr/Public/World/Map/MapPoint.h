// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Map/SavePoint.h"
#include "MapPoint.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API AMapPoint : public ASavePoint
{
	GENERATED_BODY()
	
public:
	AMapPoint(const FObjectInitializer& ObjectInitializer);
	
	/* SaveInterface */
	virtual void LoadActor_Implementation() override;
	/* SaveInterface */
	
	UPROPERTY(EditAnywhere, Category="Leyr")
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere, Category="Leyr")
	FName DestinationPlayerStartTag;

protected:
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
