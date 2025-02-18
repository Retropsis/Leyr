// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadMenu.generated.h"

class UMVVM_LoadFile;
/**
 * 
 */
UCLASS()
class LEYR_API UMVVM_LoadMenu : public UMVVMViewModelBase
{
	GENERATED_BODY()

	
public:
	void InitializeLoadSlots();

	UFUNCTION(BlueprintPure)
	UMVVM_LoadFile* GetLoadSlotViewModelByIndex(int32 Index) const;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadFile> LoadSlotViewModelClass;

	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 SlotIndex, const FString& EnteredName);

	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 SlotIndex);

private:
	UPROPERTY()
	TMap<int32, UMVVM_LoadFile*> LoadSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadFile> LoadSlot_0;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadFile> LoadSlot_1;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadFile> LoadSlot_2;
};
