// @ Retropsis 2024-2025.

#include "UI/Map/RoomTile.h"
#include "Components/Image.h"

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

void URoomTile::UnveilRoomTile()
{
	Image_Room->SetBrush(Brush_Unexplored);
	SetBordersUnexplored(RoomCoordinates, RoomSize);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	SetRoomState(ERoomState::Unexplored);
}

void URoomTile::ExploreRoomTile()
{
	HandleOccupiedAnimation(true);
	SetBorders(RoomCoordinates, RoomSize);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	SetRoomState(ERoomState::Explored);
	RevealRoomTile();
}

void URoomTile::RevealRoomTile()
{
	HandleRevealAnimation();
	
	switch (RoomType) {
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
	for (const EDoorPlacement Door : Subdivision.Doors)
	{
		switch (Door)
		{
		case EDoorPlacement::None:
			break;
		case EDoorPlacement::Top:
			Image_BorderTop->SetBrush(Brush_DoorBorder_H);
			Image_BorderTop->SetVisibility(ESlateVisibility::Visible);
			break;
		case EDoorPlacement::Bottom:
			Image_BorderBottom->SetBrush(Brush_DoorBorder_H);
			Image_BorderBottom->SetVisibility(ESlateVisibility::Visible);
			break;
		case EDoorPlacement::Left:
			Image_BorderLeft->SetBrush(Brush_DoorBorder_V);
			Image_BorderLeft->SetVisibility(ESlateVisibility::Visible);
			break;
		case EDoorPlacement::Right:
			Image_BorderRight->SetBrush(Brush_DoorBorder_V);
			Image_BorderRight->SetVisibility(ESlateVisibility::Visible);
			break;
		}
	}
}

void URoomTile::SetBorders(const FIntPoint& Coordinates, const FIntPoint& Size) const
{
	if (Coordinates.X == 1)
	{
		Image_BorderLeft->SetBrush(Brush_Border_V);
		Image_BorderLeft->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.X == Size.X)
	{
		Image_BorderRight->SetBrush(Brush_Border_V);
		Image_BorderRight->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.Y == Size.Y)
	{
		Image_BorderBottom->SetBrush(Brush_Border_H);
		Image_BorderBottom->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.Y == 1)
	{
		Image_BorderTop->SetBrush(Brush_Border_H);
		Image_BorderTop->SetVisibility(ESlateVisibility::Visible);
	}
}

void URoomTile::SetBordersUnexplored(const FIntPoint& Coordinates, const FIntPoint& Size) const
{
	if (Coordinates.X == 1)
	{
		Image_BorderLeft->SetBrush(Brush_Border_Undiscovered_V);
		Image_BorderLeft->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.X == Size.X)
	{
		Image_BorderRight->SetBrush(Brush_Border_Undiscovered_V);
		Image_BorderRight->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.Y == Size.Y)
	{
		Image_BorderBottom->SetBrush(Brush_Border_Undiscovered_H);
		Image_BorderBottom->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.Y == 1)
	{
		Image_BorderTop->SetBrush(Brush_Border_Undiscovered_H);
		Image_BorderTop->SetVisibility(ESlateVisibility::Visible);
	}
}
