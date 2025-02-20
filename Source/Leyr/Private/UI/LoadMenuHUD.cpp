// @ Retropsis 2024-2025.

#include "UI/LoadMenuHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/MVVM_LoadMenu.h"
#include "UI/Widget/LoadMenuWidget.h"

void ALoadMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	LoadMenuViewModel = NewObject<UMVVM_LoadMenu>(this, LoadMenuViewModelClass);
	LoadMenuViewModel->InitializeLoadSlots();

	LoadMenuWidget = CreateWidget<ULoadMenuWidget>(GetWorld(), LoadMenuWidgetClass);
	LoadMenuWidget->AddToViewport();
	LoadMenuWidget->BlueprintInitializeWidget();

	LoadMenuViewModel->LoadData();
}
