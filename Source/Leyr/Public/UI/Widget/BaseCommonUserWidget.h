// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "BaseCommonUserWidget.generated.h"

class UCommonActivatableWidget;
/**
 * 
 */
UCLASS()
class LEYR_API UBaseCommonUserWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCommonActivatableWidget> HUDStack;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCommonActivatableWidget> CharacterMenuStack;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCommonActivatableWidget> InventoryStack;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCommonActivatableWidget> HUDStackClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCommonActivatableWidget> CharacterMenuStackClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCommonActivatableWidget> InventoryStackClass;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
	UFUNCTION(BlueprintImplementableEvent)
	void InitializeStacks();
};
