// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineComponentActor.generated.h"

class USplineComponent;

UCLASS()
class LEYR_API ASplineComponentActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASplineComponentActor();

	UPROPERTY(EditAnywhere)
	TObjectPtr<USplineComponent> SplineComponent;

	FORCEINLINE USplineComponent* GetSplineComponent() { return SplineComponent; }
};
