// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "Inventory/ContainerComponent.h"
#include "Inventory/Data/InventoryData.h"
#include "Container.generated.h"

class UContainerComponent;
class UBoxComponent;

UCLASS()
class LEYR_API AContainer : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AContainer();
	virtual void Interact_Implementation(AActor* InteractingActor) override;

	FORCEINLINE int32 GetSlotCountContainer() const { return ContainerComponent->GetSlotCount(); }
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Container")
	int32 SlotCount = 3;

	UPROPERTY(EditAnywhere, Category="Container")
	TObjectPtr<UBoxComponent> OverlapBox;

	UPROPERTY(EditAnywhere, Category="Container")
	TObjectPtr<UContainerComponent> ContainerComponent;

	UPROPERTY(EditAnywhere, Category="Container")
	EContainerSubType ContainerSubType = EContainerSubType::Chest;
};
