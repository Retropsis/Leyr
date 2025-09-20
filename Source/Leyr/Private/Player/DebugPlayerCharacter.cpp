// @ Retropsis 2024-2025.

#include "Player/DebugPlayerCharacter.h"

#include "Game/BaseGameplayTags.h"
#include "Game/LeyrGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacterController.h"
#include "UI/Controller/InventoryWidgetController.h"

void ADebugPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	const ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(this));
	if (LeyrGameMode == nullptr) return;
		
	if(APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(Controller))
	{
		if(UInventoryWidgetController* IC = PlayerCharacterController->GetInventoryWidgetController())
		{
			for (FDataTableRowHandle DebugEquippedItem : DebugEquippedItems)
			{
				FInventoryItemData ItemToEquip = LeyrGameMode->FindItemDataByRowName(DebugEquippedItem.RowName);
				GetPlayerInventory()->ServerAddItem(ItemToEquip);
				IC->EquipButtonPressed(ItemToEquip);
			}
		}
	}
}
