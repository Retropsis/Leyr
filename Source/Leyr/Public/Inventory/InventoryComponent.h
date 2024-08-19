// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

class UItemData;
class AItem;

USTRUCT(BlueprintType)
struct FInventoryItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 ID = 0;
	
	UPROPERTY(BlueprintReadWrite)
	int32 Quantity = 0;
	
	UPROPERTY(BlueprintReadWrite)
	int32 StackSize = 0;

	UPROPERTY(BlueprintReadWrite)
	TSoftObjectPtr<UItemData> Asset = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float Health = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float MaxHealth = 0.f;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AItem> ItemClass;
	
	UPROPERTY(BlueprintReadWrite)
	int32 Ammunition = 0;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MaxAmmunition = 0;
};

UENUM(BlueprintType)
enum class EContainerType : uint8
{
	Inventory UMETA(DisplayName="Inventory"),
	Hotbar UMETA(DisplayName="Hotbar"),
	Storage UMETA(DisplayName="Storage"),
	Equipment UMETA(DisplayName="Equipment")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEYR_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerAddItem(FInventoryItemData ItemToAdd);
	
	UFUNCTION(Server, Reliable)
	void OnSlotDrop(UInventoryComponent* SourceInventory, int32 SourceSlotIndex, int32 TargetSlotIndex);
	
	bool IsSlotEmpty(int32 SlotIndex);
	FInventoryItemData GetItemAtIndex(int32 SlotIndex);
	void UpdateInventorySlotUI(int32 SlotIndex, const FInventoryItemData& ItemData) const;
	void SetInventorySize(int32 Size);

	virtual void TransferItem(UInventoryComponent* TargetInventory, int32 SourceSlotIndex, int32 TargetSlotIndex);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventoryItemData> Items;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EContainerType ContainerType = EContainerType::Inventory;

protected:
	virtual void BeginPlay() override;
	virtual bool AddItemToIndex(FInventoryItemData InventoryItemData, int32 SourceSlotIndex, int32 TargetSlotIndex);
	virtual bool RemoveItemAtIndex(int32 SlotIndex);
	virtual void HandleOnSlotDrop(UInventoryComponent* SourceInventory, int32 SourceSlotIndex, int32 TargetSlotIndex) {}

private:
	bool FindEmptySlot(int32& EmptySlotIndex);
	void AddItem(const FInventoryItemData& ItemToAdd);
};
