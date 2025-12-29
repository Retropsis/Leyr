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
	if (Coordinates.X == 1)
	{
		if (Coordinates.Y == 1 && Coordinates.Y == Size.Y) Image_BorderLeft->SetBrush(Brush_Border_V_TB);
		else if (Coordinates.Y == 1) Image_BorderLeft->SetBrush(Brush_Border_V_T);
		else if (Coordinates.Y == Size.Y) Image_BorderLeft->SetBrush(Brush_Border_V_B);
		else Image_BorderLeft->SetBrush(Brush_Border_V);
		Image_BorderLeft->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.X == Size.X)
	{
		if (Coordinates.Y == 1 && Coordinates.Y == Size.Y) Image_BorderRight->SetBrush(Brush_Border_V_TB);
		else if (Coordinates.Y == 1) Image_BorderRight->SetBrush(Brush_Border_V_T);
		else if (Coordinates.Y == Size.Y) Image_BorderRight->SetBrush(Brush_Border_V_B);
		else Image_BorderRight->SetBrush(Brush_Border_V);
		Image_BorderRight->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.Y == Size.Y)
	{
		if (Coordinates.X == 1 && Coordinates.X == Size.X) Image_BorderBottom->SetBrush(Brush_Border_H_LR);
		else if (Coordinates.X == 1) Image_BorderBottom->SetBrush(Brush_Border_H_L);
		else if (Coordinates.X == Size.X) Image_BorderBottom->SetBrush(Brush_Border_H_R);
		else Image_BorderBottom->SetBrush(Brush_Border_H);
		Image_BorderBottom->SetVisibility(ESlateVisibility::Visible);
	}
	if (Coordinates.Y == 1)
	{
		if (Coordinates.X == 1 && Coordinates.X == Size.X) Image_BorderTop->SetBrush(Brush_Border_H_LR);
		else if (Coordinates.X == 1) Image_BorderTop->SetBrush(Brush_Border_H_L);
		else if (Coordinates.X == Size.X) Image_BorderTop->SetBrush(Brush_Border_H_R);
		else Image_BorderTop->SetBrush(Brush_Border_H);
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
