// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "Data/ContainerData.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "Interaction/SaveInterface.h"
#include "Inventory/ContainerComponent.h"
#include "Inventory/Data/InventoryData.h"
#include "Container.generated.h"

class UContainerData;
class UContainerComponent;
class UBoxComponent;

UCLASS()
class LEYR_API AContainer : public APaperFlipbookActor, public IInteractionInterface, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	AContainer();

	/* Save Interface */
	virtual void LoadActor_Implementation() override;
	/* Save Interface */

	#if WITH_EDITOR
		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

	// TODO: This call is Server in the course
	virtual void Interact_Implementation(AActor* InteractingActor) override;
	bool IsContainerAccessed() const { return InteractingActors.Num() > 0; }

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleContainerLid(bool bOpen);
	
	void BuildContainerLoot();
	
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

	UPROPERTY(SaveGame)
	TArray<FInventoryItemData> Items;
	
	UPROPERTY(SaveGame)
	bool bInitialized = false;

	UPROPERTY(EditInstanceOnly, Category="Container|Initialization")
	UContainerData* ContainerData = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Container|Initialization")
	TObjectPtr<UDataTable> ItemDataTable;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Container")
	int32 SlotCount = 3;

	UPROPERTY(EditAnywhere, Category="Container")
	TObjectPtr<UBoxComponent> OverlapBox;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USoundBase> OpenLidSound = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USoundBase> CloseLidSound = nullptr;

private:
	UPROPERTY()
	TArray<AActor*> InteractingActors;
};
