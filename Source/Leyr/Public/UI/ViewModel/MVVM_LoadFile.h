// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadFile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);

/**
 * 
 */
UCLASS()
class LEYR_API UMVVM_LoadFile : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	UPROPERTY()
	FString PlayerName = FString();
	
	UPROPERTY()
	FString LoadSlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;
	
	void InitializeSlot();
};
