// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadMenuSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButton, bool, bEnable);

/**
 * 
 */
UCLASS()
class LEYR_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;
	
	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButton EnableSelectSlotButton;
	
	void InitializeSlot();

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	UPROPERTY()
	FName PlayerStartTag;
	
	UPROPERTY()
	FString MapAssetName;

	UPROPERTY()
	int32 SlotIndex;

	void SetPlayerName(FString InPlayerName);
	void SetMapName(FString InMapName);
	void SetLoadSlotName(FString InLoadSlotName);
	void SetPlayerLevel(int32 InPlayerLevel);

	FString GetPlayerName() const { return PlayerName; }
	FString GetMapName() const { return MapName; }
	FString GetLoadSlotName() const { return LoadSlotName; }
	int32 GetPlayerLevel() const { return PlayerLevel; }
	
private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
	FString PlayerName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
	int32 PlayerLevel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
	FString LoadSlotName;
};
