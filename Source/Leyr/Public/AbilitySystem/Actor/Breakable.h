// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "Interaction/InteractionInterface.h"
#include "Breakable.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class LEYR_API ABreakable : public APaperFlipbookActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ABreakable();
	virtual void Interact_Implementation() override;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Platform")
	TObjectPtr<UBoxComponent> HitBox;
	
	UPROPERTY(EditDefaultsOnly, Category="Breakable")
	int32 Health = 3;

	int32 Index = 1;
	
private:
	
};
