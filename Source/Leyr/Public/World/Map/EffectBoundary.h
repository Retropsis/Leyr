// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectBoundary.generated.h"

class UBoxComponent;

UCLASS()
class LEYR_API AEffectBoundary : public AActor
{
	GENERATED_BODY()
	
public:	
	AEffectBoundary();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> EnteringBoundary;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> EnteringBoundaryVisualizer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> OverlappingActorClass;
};
