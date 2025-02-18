// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoadMenuHUD.generated.h"

class UMVVM_LoadMenu;
class ULoadMenuWidget;
/**
 * 
 */
UCLASS()
class LEYR_API ALoadMenuHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadMenuWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULoadMenuWidget> LoadMenuWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadMenu> LoadMenuViewModelClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadMenu> LoadMenuViewModel;
	
protected:
	virtual void BeginPlay() override;
};
