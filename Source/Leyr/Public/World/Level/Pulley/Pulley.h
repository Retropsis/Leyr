// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pulley.generated.h"

class UBoxComponent;

UCLASS()
class LEYR_API APulley : public AActor
{
	GENERATED_BODY()
	
public:	
	APulley();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> RopeHitBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> WeightHitBox;
};
