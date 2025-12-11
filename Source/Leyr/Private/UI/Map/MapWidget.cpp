// @ Retropsis 2024-2025.

#include "UI/Map/MapWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
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

	for (const FRoomData& Room : MapData)
	{
		URoomTile* RoomTile = CreateWidget<URoomTile>(this, RoomTileClass);
		RoomTile->SetRoomState(ERoomState::Undiscovered);
		RoomTile->SetRoomType(Room.RoomType);
		RoomTile->SetVisibility(ESlateVisibility::Collapsed);
		CanvasPanel->AddChild(RoomTile);
		RoomTiles.Emplace(Room.RoomName, RoomTile);

		UCanvasPanelSlot* MapCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(RoomTile);
		MapCPS->SetSize(FVector2D(Room.RoomSize.X * RoomTileSize, Room.RoomSize.Y * RoomTileSize));
		MapCPS->SetPosition(FVector2D{ Room.RoomCoordinates.X * RoomTileSize + CanvasCenter.X, - Room.RoomCoordinates.Y * RoomTileSize + CanvasCenter.Y });
		RoomTile->SetOriginalPositionInCanvas(FVector2D{ Room.RoomCoordinates.X * RoomTileSize + CanvasCenter.X, - Room.RoomCoordinates.Y * RoomTileSize + CanvasCenter.Y });
	}
}

void UMapWidget::UpdateRoomTile(const FName& RoomName, const ERoomUpdateType& UpdateType)
{
	if (!RoomTiles.Contains(RoomName))
	{
		UE_LOG(LogTemp, Error, TEXT("Room name %s not found"), *RoomName.ToString());
		return;
	}
	
	if (URoomTile* RoomTile = *RoomTiles.Find(RoomName))
	{
		RoomTile->UpdateRoomTile(UpdateType);
		const FVector2D CurrentRoomTilePosition = RoomTile->GetOriginalPositionInCanvas();
		// GEngine->AddOnScreenDebugMessage(-1, 90.f, FColor::Red, FString::Printf(TEXT("CurrentRoomTilePosition: %s"), *CurrentRoomTilePosition.ToString()));
		// StartInterpolation(UpdateType, CurrentRoomTilePosition);
		FTimerHandle InterpTimer;
		GetWorld()->GetTimerManager().SetTimer(InterpTimer, [this, UpdateType, CurrentRoomTilePosition] ()
		{
			StartInterpolation(UpdateType, CurrentRoomTilePosition);
			// GEngine->AddOnScreenDebugMessage(-1, 90.f, FColor::Red, FString::Printf(TEXT("CurrentRoomTilePosition: %s"), *CurrentRoomTilePosition.ToString()));
		}, .05f, false);
	}
}

void UMapWidget::StartInterpolation(const ERoomUpdateType& UpdateType, const FVector2D& RoomTilePosition)
{
	if (UpdateType != ERoomUpdateType::PlayerLeaving)
	{
		CanvasCenter = CanvasPanel->GetCachedGeometry().GetLocalSize() / 2.f;
		TargetRoomOffset = CanvasCenter - RoomTilePosition;
		bShouldInterpolate = true;
	}
}

void UMapWidget::RedrawMap(float DeltaSecond)
{
	if (RoomTiles.IsEmpty() || !bShouldInterpolate) return;
	
	CurrentRoomOffset = FMath::Vector2DInterpTo(CurrentRoomOffset , TargetRoomOffset, DeltaSecond, 1.f);
	if (CurrentRoomOffset.Equals(TargetRoomOffset, 1.f))
	{
		CurrentRoomOffset = TargetRoomOffset;
		bShouldInterpolate = false;
	}
	for (const auto RoomTile : RoomTiles)
	{
		UCanvasPanelSlot* MapCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(RoomTile.Value);
		// MapCPS->SetSize(FVector2D(RoomTile.Value.RoomSize.X * RoomTileSize, RoomTile.Value.RoomSize.Y * RoomTileSize));
		MapCPS->SetPosition(FVector2D{ RoomTile.Value->GetOriginalPositionInCanvas().X + CurrentRoomOffset.X, RoomTile.Value->GetOriginalPositionInCanvas().Y + CurrentRoomOffset.Y});
	}
}
