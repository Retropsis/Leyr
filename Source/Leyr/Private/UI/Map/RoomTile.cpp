// @ Retropsis 2024-2025.

#include "UI/Map/RoomTile.h"
#include "Components/Image.h"

void URoomTile::SetUndiscoveredTexture()
{
	RoomState = ERoomState::Undiscovered;
	Image_Room->SetBrush(Brush_Undiscovered);
}

void URoomTile::SetDiscoveredTexture()
{
	RoomState = ERoomState::Discovered;
	Image_Room->SetBrush(Brush_Discovered);
}

void URoomTile::SetOccupiedTexture()
{
	RoomState = ERoomState::Occupied;
	Image_Room->SetBrush(Brush_Occupied);
}

void URoomTile::RevealRoom(const ERoomType NewType) const
{
	switch (NewType) {
	case ERoomType::None:
		Image_Room->SetBrush(Brush_Discovered);
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
	}
}
