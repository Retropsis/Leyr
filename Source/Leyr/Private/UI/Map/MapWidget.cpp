// @ Retropsis 2024-2025.

#include "UI/Map/MapWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "UI/Map/Room.h"
#include "UI/Map/RoomTile.h"

void UMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	RedrawMap(InDeltaTime);
}

void UMapWidget::ConstructMapCanvas(const TArray<FRoomData>& MapData)
{
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
		MapCPS->SetSize(FVector2D(RoomData.RoomSize.X * RoomTileSize_X, RoomData.RoomSize.Y * RoomTileSize_Y));
		MapCPS->SetPosition(FVector2D{ RoomData.RoomCoordinates.X * RoomTileSize_X + CanvasCenter.X, - RoomData.RoomCoordinates.Y * RoomTileSize_Y + CanvasCenter.Y });
		
		Room->SetOriginalPositionInCanvas(FVector2D{ RoomData.RoomCoordinates.X * RoomTileSize_X + CanvasCenter.X, - RoomData.RoomCoordinates.Y * RoomTileSize_Y + CanvasCenter.Y });
	}
}

void UMapWidget::EnteringRoom(const FRoomData& RoomData, const ERoomUpdateType& UpdateType, const FIntPoint& PlayerCoordinates)
{
	if (!Rooms.Contains(RoomData.RoomName))
	{
		UE_LOG(LogTemp, Error, TEXT("Room name %s not found"), *RoomData.RoomName.ToString());
		return;
	}

	Text_RoomName->SetText(FText::FromName(RoomData.RoomName));
	
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

		InterpolateMinimapToCenter(UpdateType, Room);
	}
}

void UMapWidget::InterpolateMinimapToCenter(const ERoomUpdateType& UpdateType, URoom* Room)
{
	const UCanvasPanelSlot* RoomCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(Room);
	const FVector2D CurrentRoomTilePosition = Room->GetOriginalPositionInCanvas() + RoomCPS->GetSize() / 2.f;
	FTimerHandle InterpTimer;
	GetWorld()->GetTimerManager().SetTimer(InterpTimer, [this, UpdateType, CurrentRoomTilePosition] ()
	{
		StartInterpolation(CurrentRoomTilePosition);
	}, .05f, false);
}

void UMapWidget::LeavingRoom(const FRoomData& RoomData, const FIntPoint& PlayerCoordinates)
{
	if (!Rooms.Contains(RoomData.RoomName)) return;
	
	if (URoom* Room = *Rooms.Find(RoomData.RoomName))
	{
		Room->LeavingRoom(PlayerCoordinates, RoomData);
	}
}

void UMapWidget::UnveilRoom(const FRoomData& RoomData)
{
	if (!Rooms.Contains(RoomData.RoomName))
	{
		UE_LOG(LogTemp, Error, TEXT("Room Tile name %s not found"), *RoomData.RoomName.ToString());
		return;
	}
	
	if (URoom* Room = *Rooms.Find(RoomData.RoomName))
	{
		Room->UnveilingRoom(FIntPoint(-1, -1), RoomData);
	}
}

void UMapWidget::UpdateRoomTileAt(const FRoomData& RoomData, const ERoomUpdateType& UpdateType, const FIntPoint& PlayerCoordinates)
{
	if (!Rooms.Contains(RoomData.RoomName))
	{
		UE_LOG(LogTemp, Error, TEXT("Room Tile name %s not found"), *RoomData.RoomName.ToString());
		return;
	}
	
	if (URoom* Room = *Rooms.Find(RoomData.RoomName))
	{
		Room->UpdateRoomTile(UpdateType, PlayerCoordinates, RoomData);
	}
}

void UMapWidget::UpdateCompletionText(const float CompletionRate) const
{
	Text_MapCompletion->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), CompletionRate)));
}

void UMapWidget::StartInterpolation(const FVector2D& RoomTilePosition)
{
	CanvasCenter = CanvasPanel->GetCachedGeometry().GetLocalSize() / 2.f;
	TargetRoomOffset = CanvasCenter - RoomTilePosition;
	bShouldInterpolate = true;
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
