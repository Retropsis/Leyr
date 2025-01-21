// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Data/ItemData.h"
#include "InventoryComponent.generated.h"

struct FGameplayTag;
class UItemData;
class AItem;

USTRUCT(BlueprintType, Blueprintable)
struct FInventoryItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ID = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Name = FName();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Quantity = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 StackSize = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UItemData> Asset = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FGameplayTag> Abilities;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Categories="Equipment"))
	FGameplayTag EquipmentSlot = FGameplayTag();
};

UENUM(BlueprintType)
enum class EContainerType : uint8
{
	Inventory UMETA(DisplayName="Inventory"),
	Hotbar UMETA(DisplayName="Hotbar"),
	Container UMETA(DisplayName="Container"),
	Equipment UMETA(DisplayName="Equipment")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemUpdated, EContainerType, ContainerType, int32, SlotIndex, FInventoryItemData, Item);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEYR_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnItemUpdated OnItemUpdated;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerAddItem(FInventoryItemData ItemToAdd);
	
	UFUNCTION(Server, Reliable)
	void OnSlotDrop(UInventoryComponent* SourceInventory, int32 SourceSlotIndex, int32 TargetSlotIndex);
	
	bool IsSlotEmpty(int32 SlotIndex);
	FInventoryItemData GetItemAtIndex(int32 SlotIndex);
	void UpdateInventorySlotUI(int32 SlotIndex, const FInventoryItemData& ItemData) const;
	void ResetInventorySlotUI(int32 SlotIndex, const FInventoryItemData& ItemData) const;
	void SetInventorySize(int32 Size);

	virtual void TransferItem(UInventoryComponent* TargetInventory, int32 SourceSlotIndex, int32 TargetSlotIndex);
	virtual void TransferItemToEmptySlot(UInventoryComponent* TargetInventory, int32 SourceSlotIndex);
	virtual bool UseItem(UItemData* Asset, int32 Amount);
	virtual bool UpdateItemInputTag(FInventoryItemData ItemData, FGameplayTag InputTag) { return false; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventoryItemData> Items;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EContainerType ContainerType = EContainerType::Inventory;

	FORCEINLINE int32 GetSlotCount() const { return Items.Num(); }
	FORCEINLINE void SetSlotCount(const int32 NewCount) { Items.SetNum(NewCount); }

protected:
	virtual void BeginPlay() override;
	virtual bool AddItemToIndex(FInventoryItemData InventoryItemData, int32 SourceSlotIndex, int32 TargetSlotIndex);
	virtual bool RemoveItemAtIndex(int32 SlotIndex);
	virtual void HandleOnSlotDrop(UInventoryComponent* SourceInventory, int32 SourceSlotIndex, int32 TargetSlotIndex) {}

private:
	bool FindEmptySlot(int32& EmptySlotIndex);
	void AddItem(FInventoryItemData& ItemToAdd, bool bItemStackWasSplit = false);
};
