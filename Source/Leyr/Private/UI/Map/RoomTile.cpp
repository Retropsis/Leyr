// @ Retropsis 2024-2025.

#include "UI/Map/RoomTile.h"
#include "Components/Image.h"

void URoomTile::UpdateRoomTile(const ERoomUpdateType& UpdateType)
{	
	switch (UpdateType) {
	case ERoomUpdateType::None:
		break;
	case ERoomUpdateType::PlayerDiscovering:
		SetVisibility(ESlateVisibility::HitTestInvisible);
		RevealRoom();
		HandleRevealAnimation();
		HandleOccupiedAnimation(true);
		SetRoomState(ERoomState::Discovered);
		break;
	case ERoomUpdateType::PlayerEntering:
		HandleOccupiedAnimation(true);
		break;
	case ERoomUpdateType::PlayerLeaving:
		HandleOccupiedAnimation(false);
		break;
	}
}

void URoomTile::RevealRoom() const
{
	switch (RoomType) {
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
	case ERoomType::ExitRoom:
		Image_Room->SetBrush(Brush_ExitRoom);
		break;
	}
}
