// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "PhysicsConstrainedActor.generated.h"

class UBoxComponent;
class UPhysicsConstraintComponent;

UCLASS()
class LEYR_API APhysicsConstrainedActor : public APaperFlipbookActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	APhysicsConstrainedActor();
	virtual void InteractHit_Implementation(AActor* InteractingActor) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPhysicsConstraintComponent> PhysicsConstraint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> HitBoxComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> WeightBoxComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBreak = false;
};
