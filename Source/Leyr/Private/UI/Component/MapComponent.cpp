// @ Retropsis 2024-2025.

#include "UI/Component/MapComponent.h"
#include "Blueprint/UserWidget.h"
#include "Data/MapData.h"
#include "UI/Map/MapWidget.h"

UMapComponent::UMapComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMapComponent::UpdateMap(const FMapUpdateData& Data) const
{
	switch (Data.UpdateType) {
	case EMapUpdateType::None:
		break;
	case EMapUpdateType::PlayerEntering:
		GEngine->AddOnScreenDebugMessage(1001, 30.f, FColor::Green, FString::Printf(TEXT("%s"), *Data.RoomName.ToString()));
		GEngine->AddOnScreenDebugMessage(1002, 30.f, FColor::Green, FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(Data.RoomType)));
		GEngine->AddOnScreenDebugMessage(1003, 30.f, FColor::Green, FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(Data.UpdateType)));
		GEngine->AddOnScreenDebugMessage(1004, 30.f, FColor::Green, FString::Printf(TEXT("X: %d - Y: %d"), Data.RoomCoordinates.X, Data.RoomCoordinates.Y));
		MapWidget->UpdateRoomTile(Data);
		break;
	case EMapUpdateType::PlayerLeaving:
		GEngine->AddOnScreenDebugMessage(2001, 30.f, FColor::Orange, FString::Printf(TEXT("%s"), *Data.RoomName.ToString()));
		GEngine->AddOnScreenDebugMessage(2002, 30.f, FColor::Orange, FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(Data.UpdateType)));
		GEngine->AddOnScreenDebugMessage(2003, 30.f, FColor::Orange, FString::Printf(TEXT("X: %d - Y: %d"), Data.RoomCoordinates.X, Data.RoomCoordinates.Y));
		MapWidget->UpdateRoomTile(Data);
		break;
	case EMapUpdateType::PlayerDiscovering:
		break;
	}
}

void UMapComponent::BeginPlay()
{
	Super::BeginPlay();
	ConstructMap();
}

void UMapComponent::ConstructMap()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("Map Component should have PlayerController as Owner."));
	if (!OwningController->IsLocalController()) return;

	MapWidget = CreateWidget<UMapWidget>(OwningController.Get(), MapWidgetClass);
	MapWidget->AddToViewport();
	MapWidget->SetDesiredSizeInViewport(FVector2D{ 400.f, 400.f });
	const FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
	MapWidget->SetPositionInViewport(FVector2D{ ViewportSize.X /2.f + 600.f, ViewportSize.Y / 2.f + 200.f });
	// CloseInventoryMenu();
}
