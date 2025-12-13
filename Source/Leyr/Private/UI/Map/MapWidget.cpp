// @ Retropsis 2024-2025.

#include "UI/Map/MapWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/Map/Room.h"
#include "UI/Map/RoomTile.h"

void UMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	RedrawMap(InDeltaTime);
}

void UMapWidget::ConstructMapCanvas(const TArray<FRoomData>& MapData)
{
	// const FVector2D CanvasCenter = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY()) / 2.f;
	if (!IsValid(CanvasPanel))
	{
		UE_LOG(LogTemp, Error, TEXT("CanvasPanel is invalid"));
		return;
	}

	for (const FRoomData& RoomData : MapData)
	{
		URoom* Room = CreateWidget<URoom>(this, RoomClass);
		Room->SetRoomState(ERoomState::Unexplored);
		Room->SetRoomType(RoomData.RoomType);
		Room->ConstructRoom(RoomData);
		// UE_LOG(LogTemp, Warning, TEXT("Room was created with the name: %s size: (w%d, h%d), coords: (x%d, y%d) Type: %s"), *RoomData.RoomName.ToString(), RoomData.RoomSize.X, RoomData.RoomSize.Y, RoomData.RoomCoordinates.X, RoomData.RoomCoordinates.Y, *UEnum::GetValueAsString(RoomData.RoomType));

		CanvasPanel->AddChild(Room);
		Rooms.Emplace(RoomData.RoomName, Room);

		UCanvasPanelSlot* MapCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(Room);
		MapCPS->SetSize(FVector2D(RoomData.RoomSize.X * RoomTileSize, RoomData.RoomSize.Y * RoomTileSize));
		MapCPS->SetPosition(FVector2D{ RoomData.RoomCoordinates.X * RoomTileSize + CanvasCenter.X, - RoomData.RoomCoordinates.Y * RoomTileSize + CanvasCenter.Y });
		
		Room->SetOriginalPositionInCanvas(FVector2D{ RoomData.RoomCoordinates.X * RoomTileSize + CanvasCenter.X, - RoomData.RoomCoordinates.Y * RoomTileSize + CanvasCenter.Y });
	}
}

void UMapWidget::EnteringRoom(const FRoomData& RoomData, const ERoomUpdateType& UpdateType, const FIntPoint& PlayerCoordinates)
{
	if (!Rooms.Contains(RoomData.RoomName))
	{
		UE_LOG(LogTemp, Error, TEXT("Room name %s not found"), *RoomData.RoomName.ToString());
		return;
	}
	
	if (URoom* Room = *Rooms.Find(RoomData.RoomName))
	{
		if (UpdateType == ERoomUpdateType::Unveiling)
		{
			Room->UnveilingRoom(PlayerCoordinates, RoomData);
		}
		if (UpdateType == ERoomUpdateType::Entering)
		{
			Room->EnteringRoom(PlayerCoordinates, RoomData);
		}
		
		const FVector2D CurrentRoomTilePosition = Room->GetOriginalPositionInCanvas();
		FTimerHandle InterpTimer;
		GetWorld()->GetTimerManager().SetTimer(InterpTimer, [this, UpdateType, CurrentRoomTilePosition] ()
		{
			StartInterpolation(UpdateType, CurrentRoomTilePosition);
		}, .05f, false);
	}
}

void UMapWidget::LeavingRoom(const FRoomData& RoomData, const FIntPoint& PlayerCoordinates)
{
	if (!Rooms.Contains(RoomData.RoomName)) return;
	
	if (URoom* Room = *Rooms.Find(RoomData.RoomName))
	{
		Room->LeavingRoom(PlayerCoordinates, RoomData);
	}
}

void UMapWidget::UpdateRoomTileAt(const FRoomData& RoomData, const ERoomUpdateType& UpdateType, const FIntPoint& Coordinates)
{
	if (!Rooms.Contains(RoomData.RoomName))
	{
		UE_LOG(LogTemp, Error, TEXT("Room name %s not found"), *RoomData.RoomName.ToString());
		return;
	}
	
	if (URoom* Room = *Rooms.Find(RoomData.RoomName))
	{
		Room->UpdateRoomTile(UpdateType, Coordinates, RoomData);
		// const FVector2D CurrentRoomTilePosition = Room->GetOriginalPositionInCanvas();
		// FTimerHandle InterpTimer;
		// GetWorld()->GetTimerManager().SetTimer(InterpTimer, [this, UpdateType, CurrentRoomTilePosition] ()
		// {
		// 	StartInterpolation(UpdateType, CurrentRoomTilePosition);
		// }, .05f, false);
	}
}

void UMapWidget::StartInterpolation(const ERoomUpdateType& UpdateType, const FVector2D& RoomTilePosition)
{
	if (UpdateType != ERoomUpdateType::Leaving)
	{
		CanvasCenter = CanvasPanel->GetCachedGeometry().GetLocalSize() / 2.f;
		TargetRoomOffset = CanvasCenter - RoomTilePosition;
		bShouldInterpolate = true;
	}
}

void UMapWidget::RedrawMap(float DeltaSecond)
{
	if (Rooms.IsEmpty() || !bShouldInterpolate) return;
	
	CurrentRoomOffset = FMath::Vector2DInterpTo(CurrentRoomOffset , TargetRoomOffset, DeltaSecond, 1.f);
	if (CurrentRoomOffset.Equals(TargetRoomOffset, 1.f))
	{
		CurrentRoomOffset = TargetRoomOffset;
		bShouldInterpolate = false;
	}
	for (const auto Room : Rooms)
	{
		UCanvasPanelSlot* MapCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(Room.Value);
		// MapCPS->SetSize(FVector2D(RoomTile.Value.RoomSize.X * RoomTileSize, RoomTile.Value.RoomSize.Y * RoomTileSize));
		MapCPS->SetPosition(FVector2D{ Room.Value->GetOriginalPositionInCanvas().X + CurrentRoomOffset.X, Room.Value->GetOriginalPositionInCanvas().Y + CurrentRoomOffset.Y});
	}
}
