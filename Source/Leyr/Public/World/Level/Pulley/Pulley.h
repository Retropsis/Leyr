// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World/Level/Lever.h"
#include "Pulley.generated.h"

class UPhysicsConstraintComponent;
class UBoxComponent;

UCLASS()
class LEYR_API APulley : public ALever
{
	GENERATED_BODY()
	
public:	
	APulley();

	//~ Save Interface
	virtual void LoadActor_Implementation() override;
	//~ Save Interface

protected:
	virtual void BeginPlay() override;
	virtual void InteractHit_Implementation(AActor* InteractingActor) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> WeightHitBox;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPhysicsConstraintComponent> PhysicsConstraint;
};
