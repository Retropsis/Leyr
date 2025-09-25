// @ Retropsis 2024-2025.

#include "Player/DebugPlayerCharacter.h"
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
			for (const FDataTableRowHandle DebugEquippedItem : DebugEquippedItems)
			{
				const FInventoryItemData ItemToEquip = LeyrGameMode->FindItemDataByRowName(DebugEquippedItem.RowName);
				GetPlayerInventory()->ServerAddItem(ItemToEquip);
				IC->EquipButtonPressed(ItemToEquip);
			}
		}
	}

	const int32 CurrentLevel = Execute_GetCharacterLevel(this);
	const int32 NumLevelUps = DebugLevel - Execute_GetCharacterLevel(this);
	int32 XPToReward = 0;
	if(NumLevelUps > 0)
	{
		Execute_AddToPlayerLevel(this, NumLevelUps);
		
		int32 AttributePointReward = 0;
		int32 SkillPointReward = 0;

		for (int i = 0; i < NumLevelUps; ++i)
		{
			SkillPointReward += Execute_GetSkillPointsReward(this, CurrentLevel + i);
			AttributePointReward += Execute_GetAttributePointsReward(this, CurrentLevel + i);
			XPToReward += Execute_FindXPForLevel(this, CurrentLevel + i);
		}
		Execute_AddToAttributePoints(this, AttributePointReward);
		Execute_AddToSkillPoints(this, SkillPointReward);
	}
	Execute_AddToXP(this, XPToReward);
}
