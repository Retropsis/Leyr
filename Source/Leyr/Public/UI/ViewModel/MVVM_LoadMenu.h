// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);

class UMVVM_LoadSlot;
/**
 * 
 */
UCLASS()
class LEYR_API UMVVM_LoadMenu : public UMVVMViewModelBase
{
	GENERATED_BODY()

	
public:
	void InitializeLoadSlots();
	void LoadData();

	UPROPERTY(BlueprintAssignable)
	FSlotSelected SlotSelected;

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 SlotIndex, const FString& EnteredName);

	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed();

private:
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> SelectedSlot;
};
