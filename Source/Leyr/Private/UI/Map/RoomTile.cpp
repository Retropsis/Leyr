// @ Retropsis 2024-2025.

#include "UI/Map/RoomTile.h"
#include "Components/Image.h"

void URoomTile::NativeConstruct()
{
	Super::NativeConstruct();

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

void URoomTile::InitializeRoomTile(const FRoomData& RoomData, const FIntPoint& SubdivisionCoordinates)
{
	SetRoomType(RoomData.RoomType);
	SetRoomSize(RoomData.RoomSize);
	SetTileCoordinates(SubdivisionCoordinates);
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
			Image_BorderTop->SetBrush(Door.Value == EEntranceType::Hidden ? Brush_Border_H : Brush_DoorBorder_H);
			Image_BorderTop->SetVisibility(ESlateVisibility::Visible);
			break;
		case ESubdivisionSide::Bottom:
			Image_BorderBottom->SetBrush(Door.Value == EEntranceType::Hidden ? Brush_Border_H : Brush_DoorBorder_H);
			Image_BorderBottom->SetVisibility(ESlateVisibility::Visible);
			break;
		case ESubdivisionSide::Left:
			Image_BorderLeft->SetBrush(Door.Value == EEntranceType::Hidden ? Brush_Border_V : Brush_DoorBorder_V);
			Image_BorderLeft->SetVisibility(ESlateVisibility::Visible);
			break;
		case ESubdivisionSide::Right:
			Image_BorderRight->SetBrush(Door.Value == EEntranceType::Hidden ? Brush_Border_V : Brush_DoorBorder_V);
			Image_BorderRight->SetVisibility(ESlateVisibility::Visible);
			break;
		}
	}
}

void URoomTile::SetBorders(const FIntPoint& Coordinates, const FIntPoint& Size) const
{
	if (Coordinates.X == 0)
	{
		if (Coordinates.Y == 0 && Coordinates.Y == Size.Y - 1) Image_BorderLeft->SetBrush(Brush_Border_V_TB);
		else if (Coordinates.Y == 0) Image_BorderLeft->SetBrush(Brush_Border_V_T);
		else if (Coordinates.Y == Size.Y - 1) Image_BorderLeft->SetBrush(Brush_Border_V_B);
		else Image_BorderLeft->SetBrush(Brush_Border_V);
		Image_BorderLeft->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.X == Size.X - 1)
	{
		if (Coordinates.Y == 0 && Coordinates.Y == Size.Y - 1) Image_BorderRight->SetBrush(Brush_Border_V_TB);
		else if (Coordinates.Y == 0) Image_BorderRight->SetBrush(Brush_Border_V_T);
		else if (Coordinates.Y == Size.Y - 1) Image_BorderRight->SetBrush(Brush_Border_V_B);
		else Image_BorderRight->SetBrush(Brush_Border_V);
		Image_BorderRight->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.Y == Size.Y - 1)
	{
		if (Coordinates.X == 0 && Coordinates.X == Size.X - 1) Image_BorderBottom->SetBrush(Brush_Border_H_LR);
		else if (Coordinates.X == 0) Image_BorderBottom->SetBrush(Brush_Border_H_L);
		else if (Coordinates.X == Size.X - 1) Image_BorderBottom->SetBrush(Brush_Border_H_R);
		else Image_BorderBottom->SetBrush(Brush_Border_H);
		Image_BorderBottom->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.Y == 0)
	{
		if (Coordinates.X == 0 && Coordinates.X == Size.X - 1) Image_BorderTop->SetBrush(Brush_Border_H_LR);
		else if (Coordinates.X == 0) Image_BorderTop->SetBrush(Brush_Border_H_L);
		else if (Coordinates.X == Size.X - 1) Image_BorderTop->SetBrush(Brush_Border_H_R);
		else Image_BorderTop->SetBrush(Brush_Border_H);
		Image_BorderTop->SetVisibility(ESlateVisibility::Visible);
	}
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
