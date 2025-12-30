// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "MapFragment.generated.h"

UCLASS()
class LEYR_API AMapFragment : public AItem
{
	GENERATED_BODY()

public:
	AMapFragment();
	
	//~ Interaction Interface
	virtual void Interact_Implementation(AActor* InteractingActor) override;
	//~ Interaction Interface

private:
	UPROPERTY(EditAnywhere, Category = "Leyr")
	TArray<FName> RoomsToUnveil;
};
