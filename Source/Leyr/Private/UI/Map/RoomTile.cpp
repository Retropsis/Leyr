// @ Retropsis 2024-2025.

#include "UI/Map/RoomTile.h"
#include "World/Data/MapInfo.h"
#include "Components/Image.h"

void URoomTile::InitializeTint(const FMinimapData& Data)
{
	Brush_Explored.TintColor = Data.ExploredTint;
	
	Brush_Horizontal_Wall_Top.TintColor = Data.WallTint;
	Brush_Horizontal_Wall_Bottom.TintColor = Data.WallTint;
	Brush_Vertical_Wall_Left.TintColor = Data.WallTint;
	Brush_Vertical_Wall_Right.TintColor = Data.WallTint;
	
	Brush_Horizontal_Door_Top.TintColor = Data.DoorTint;
	Brush_Horizontal_Door_Bottom.TintColor = Data.DoorTint;
	Brush_Vertical_Door_Left.TintColor = Data.DoorTint;
	Brush_Vertical_Door_Right.TintColor = Data.DoorTint;
	
	Brush_Corner_Top_Left.TintColor = Data.CornerTint;
	Brush_Corner_Top_Right.TintColor = Data.CornerTint;
	Brush_Corner_Bottom_Left.TintColor = Data.CornerTint;
	Brush_Corner_Bottom_Right.TintColor = Data.CornerTint;
	Image_CornerTopLeft->SetBrush(Brush_Corner_Top_Left);
	Image_CornerTopRight->SetBrush(Brush_Corner_Top_Right);
	Image_CornerBottomLeft->SetBrush(Brush_Corner_Bottom_Left);
	Image_CornerBottomRight->SetBrush(Brush_Corner_Bottom_Right);
}

void URoomTile::UpdateRoomTile(const ERoomUpdateType& UpdateType)
{	
	switch (UpdateType) {
	case ERoomUpdateType::None:
		break;
	case ERoomUpdateType::Unveiling:
		UnveilRoomTile();
		break;
	case ERoomUpdateType::Exploring:
		ExploreRoomTile();
		break;
	case ERoomUpdateType::Entering:
		EnterRoomTile();
		break;
	case ERoomUpdateType::Leaving:
		LeaveRoomTile();
		break;
	case ERoomUpdateType::RevealDoor:
		break;
	}
}

void URoomTile::EnterRoomTile()
{
	HandleOccupiedAnimation(true);
}

void URoomTile::LeaveRoomTile()
{
	HandleOccupiedAnimation(false);
}

void URoomTile::InitializeRoomTile(const FRoomData& RoomData, const FIntPoint& SubdivisionCoordinates, const FMinimapData& MinimapData)
{
	SetRoomType(RoomData.RoomType);
	SetRoomSize(RoomData.RoomSize);
	SetTileCoordinates(SubdivisionCoordinates);
	InitializeTint(MinimapData);
	if (RoomData.Subdivisions.Contains(SubdivisionCoordinates))
	{
		switch (RoomData.Subdivisions[SubdivisionCoordinates].SubdivisionState) {
		case ESubdivisionState::Hidden:
			SetVisibility(ESlateVisibility::Collapsed);
			break;
		case ESubdivisionState::Unexplored:
			SetVisibility(ESlateVisibility::HitTestInvisible);
			SetBordersUnexplored(SubdivisionCoordinates, RoomSize);
			Image_Room->SetBrush(Brush_Unexplored);
			break;
		case ESubdivisionState::Explored:
			SetVisibility(ESlateVisibility::HitTestInvisible);
			SetBorders(SubdivisionCoordinates, RoomSize);
			SetRoomTileBrushByType(RoomData.RoomType);
			DrawDoors(RoomData.Subdivisions[SubdivisionCoordinates]);
			break;
		}
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void URoomTile::UnveilRoomTile()
{
	Image_Room->SetBrush(Brush_Unexplored);
	SetBordersUnexplored(TileCoordinates, RoomSize);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	SetRoomState(ERoomState::Unexplored);
}

void URoomTile::ExploreRoomTile()
{
	HandleOccupiedAnimation(true);
	SetBorders(TileCoordinates, RoomSize);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	SetRoomState(ERoomState::Explored);
	RevealRoomTile();
}

void URoomTile::RevealRoomTile()
{
	HandleRevealAnimation();
	SetRoomTileBrushByType(RoomType);
}

void URoomTile::SetRoomTileBrushByType(const ERoomType Type) const
{
	switch (Type) {
	case ERoomType::None:
		Image_Room->SetBrush(Brush_Explored);
		break;
	case ERoomType::SaveRoom:
		Image_Room->SetBrush(Brush_SaveRoom);
		break;
	case ERoomType::WarpRoom:
		Image_Room->SetBrush(Brush_WarpRoom);
		break;
	case ERoomType::BossRoom:
		Image_Room->SetBrush(Brush_BossRoom);
		break;
	case ERoomType::ExitRoom:
		Image_Room->SetBrush(Brush_ExitRoom);
		break;
	}
}

void URoomTile::DrawDoors(const FSubdivision& Subdivision) const
{
	for (const TTuple<ESubdivisionSide, EEntranceType> Door : Subdivision.Doors)
	{
		switch (Door.Key)
		{
		case ESubdivisionSide::None:
			break;
		case ESubdivisionSide::Top:
			Image_BorderTop->SetBrush(Door.Value == EEntranceType::Hidden ? Brush_Horizontal_Wall_Top : Brush_Horizontal_Door_Top);
			Image_BorderTop->SetVisibility(ESlateVisibility::Visible);
			break;
		case ESubdivisionSide::Bottom:
			Image_BorderBottom->SetBrush(Door.Value == EEntranceType::Hidden ? Brush_Horizontal_Wall_Bottom : Brush_Horizontal_Door_Bottom);
			Image_BorderBottom->SetVisibility(ESlateVisibility::Visible);
			break;
		case ESubdivisionSide::Left:
			Image_BorderLeft->SetBrush(Door.Value == EEntranceType::Hidden ? Brush_Vertical_Wall_Left : Brush_Vertical_Door_Left);
			Image_BorderLeft->SetVisibility(ESlateVisibility::Visible);
			break;
		case ESubdivisionSide::Right:
			Image_BorderRight->SetBrush(Door.Value == EEntranceType::Hidden ? Brush_Vertical_Wall_Right : Brush_Vertical_Door_Right);
			Image_BorderRight->SetVisibility(ESlateVisibility::Visible);
			break;
		}
	}
}

void URoomTile::SetBorders(const FIntPoint& Coordinates, const FIntPoint& Size) const
{
	if (Coordinates.X == 0)
	{
		Image_BorderLeft->SetBrush(Brush_Vertical_Wall_Left);
		// Image_BorderLeft->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.X == Size.X - 1)
	{
		Image_BorderRight->SetBrush(Brush_Vertical_Wall_Right);
		// Image_BorderRight->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.Y == Size.Y - 1)
	{
		Image_BorderBottom->SetBrush(Brush_Horizontal_Wall_Bottom);
		// Image_BorderBottom->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.Y == 0)
	{
		Image_BorderTop->SetBrush(Brush_Horizontal_Wall_Top);
		// Image_BorderTop->SetVisibility(ESlateVisibility::Visible);
	}
	
	if (Coordinates.X == 0 && Coordinates.Y == 0) Image_CornerTopLeft->SetVisibility(ESlateVisibility::Visible);
	if (Coordinates.X == 0 && Coordinates.Y == Size.Y - 1) Image_CornerBottomLeft->SetVisibility(ESlateVisibility::Visible);
	if (Coordinates.X == Size.X - 1 && Coordinates.Y == 0) Image_CornerTopRight->SetVisibility(ESlateVisibility::Visible);
	if (Coordinates.X == Size.X - 1 && Coordinates.Y == Size.Y - 1) Image_CornerBottomRight->SetVisibility(ESlateVisibility::Visible);
}

void URoomTile::SetBordersUnexplored(const FIntPoint& Coordinates, const FIntPoint& Size) const
{
	if (Coordinates.X == 0)
	{
		Image_BorderLeft->SetBrush(Brush_Border_Undiscovered_V);
		Image_BorderLeft->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.X == Size.X - 1)
	{
		Image_BorderRight->SetBrush(Brush_Border_Undiscovered_V);
		Image_BorderRight->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.Y == Size.Y - 1)
	{
		Image_BorderBottom->SetBrush(Brush_Border_Undiscovered_H);
		Image_BorderBottom->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.Y == 0)
	{
		Image_BorderTop->SetBrush(Brush_Border_Undiscovered_H);
		Image_BorderTop->SetVisibility(ESlateVisibility::Visible);
	}
}
