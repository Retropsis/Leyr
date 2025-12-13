// @ Retropsis 2024-2025.

#include "UI/Map/Room.h"
#include "Components/GridPanel.h"
#include "UI/Map/RoomTile.h"

void URoom::ConstructRoom(const FRoomData& RoomData)
{
	// UE_LOG(LogTemp, Warning, TEXT("Constructing room %s with size of (w: %d,h: %d)"), *RoomData.RoomName.ToString(), RoomData.RoomSize.X, RoomData.RoomSize.Y);
	for (int h = 1; h <= RoomData.RoomSize.Y; ++h)
	{
		for (int w = 1; w <= RoomData.RoomSize.X; ++w)
		{
			URoomTile* RoomTile = CreateWidget<URoomTile>(this, RoomTileClass);
			RoomTile->SetRoomState(ERoomState::Hidden);
			RoomTile->SetRoomType(RoomData.RoomType);
			RoomTile->SetVisibility(ESlateVisibility::Collapsed);
			RoomTile->SetRoomSize(RoomData.RoomSize);
			RoomTile->SetRoomCoordinates(FIntPoint(w, h));
			RoomSize = RoomData.RoomSize;
			RoomGrid->AddChildToGrid(RoomTile, h, w);
			RoomTiles.Emplace(FIntPoint(w - 1, h - 1), RoomTile);
			// UE_LOG(LogTemp, Warning, TEXT("Constructing a room tile at (x: %d, y: %d)"), w, h);
		}
	}
}

void URoom::EnteringRoom(const FIntPoint& PlayerCoordinates, const FRoomData& RoomData)
{
	for (const TTuple<FIntPoint, URoomTile*>& RoomTile : RoomTiles)
	{		
		if (RoomData.Subdivisions.Contains(RoomTile.Key) && RoomData.Subdivisions.Find(RoomTile.Key)->SubdivisionState == ESubdivisionState::Explored)
		{
			if (PlayerCoordinates == RoomTile.Key)
			{
				RoomTile.Value->EnterRoomTile();
			}
		}
	}
}

void URoom::UnveilingRoom(const FIntPoint& PlayerCoordinates, const FRoomData& RoomData)
{
	for (const TTuple<FIntPoint, URoomTile*>& RoomTile : RoomTiles)
	{		
		if (RoomData.Subdivisions.Contains(RoomTile.Key) && RoomData.Subdivisions.Find(RoomTile.Key)->SubdivisionState == ESubdivisionState::Unexplored)
		{
			RoomTile.Value->UnveilRoomTile();
			if (PlayerCoordinates == RoomTile.Key)
			{
				RoomTile.Value->ExploreRoomTile();
			}
		}
	}
}

void URoom::LeavingRoom(const FIntPoint& PlayerCoordinates, const FRoomData& RoomData)
{
	for (const TTuple<FIntPoint, URoomTile*>& RoomTile : RoomTiles)
	{
		RoomTile.Value->LeaveRoomTile();
	}
}

void URoom::UpdateRoomTile(const ERoomUpdateType& UpdateType, const FIntPoint& Coordinates, const FRoomData& RoomData)
{
	if (URoomTile* RoomTile = Cast<URoomTile>(RoomGrid->GetChildAt(Coordinates.X + Coordinates.Y * RoomSize.X)))
	{
		// if (RoomData.Subdivisions.Contains(Coordinates) && RoomData.Subdivisions.Find(Coordinates)->bWasExplored)
		// {
		// 	
		// }
		switch (UpdateType) {
		case ERoomUpdateType::None:
			break;
		case ERoomUpdateType::Entering:
			LeaveRoomTile(RoomTile);
			RoomTile->EnterRoomTile();
			break;
		case ERoomUpdateType::Leaving:
			RoomTile->LeaveRoomTile();
			break;
		case ERoomUpdateType::Exploring:
			LeaveRoomTile(RoomTile);
			RoomTile->ExploreRoomTile();
			break;
		case ERoomUpdateType::Unveiling:
			RoomTile->UnveilRoomTile();
			break;
		}
	}
}

void URoom::LeaveRoomTile(URoomTile* RoomTile)
{
	if (LastOccupiedRoomTile.IsValid())
	{
		LastOccupiedRoomTile->LeaveRoomTile();
	}
	LastOccupiedRoomTile = RoomTile;
}
