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

	// TODO: This call is Server in the course
	virtual void Interact_Implementation(AActor* InteractingActor) override;
	bool IsContainerAccessed() const { return InteractingActors.Num() > 0; }

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleContainerLid(bool bOpen);
	
	UFUNCTION(Server, Reliable)
	void ServerStopInteracting(AActor* InteractingActor);

	UFUNCTION(Server, Reliable)
	void ServerForEachActorUpdateInventorySlot(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData Item);
	
	UFUNCTION(Server, Reliable)
	void ServerForEachActorResetInventorySlot(EContainerType ContainerType, int32 SlotIndex);
	
	UFUNCTION(Server, Reliable)
	void ServerUpdateContainer();

	FORCEINLINE int32 GetSlotCountContainer() const { return Container->GetSlotCount(); }

	UPROPERTY(EditAnywhere, Category="Container")
	EContainerSubType ContainerSubType = EContainerSubType::Chest;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Container")
	TObjectPtr<UContainerComponent> Container;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Container")
	int32 SlotCount = 3;

	UPROPERTY(EditAnywhere, Category="Container")
	TObjectPtr<UBoxComponent> OverlapBox;

private:
	UPROPERTY()
	TArray<AActor*> InteractingActors;
};
