// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayerCharacter.h"
#include "UI/Controller/InventoryWidgetController.h"
#include "DebugPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API ADebugPlayerCharacter : public APlayerCharacter
{
	GENERATED_BODY()

public:
	// virtual void LoadProgress() override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug", meta=(Categories=Equipment))
	TArray<FDataTableRowHandle> DebugEquippedItems;
};
