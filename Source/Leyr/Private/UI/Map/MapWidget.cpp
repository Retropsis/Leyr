// @ Retropsis 2024-2025.

#include "UI/Map/MapWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Map/RoomTile.h"
#include "World/Map/CameraBoundary.h"

void UMapWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ConstructMap();
}

void UMapWidget::ConstructMap()
{
	TArray<AActor*> TempRooms;
	UGameplayStatics::GetAllActorsOfClass(this, ACameraBoundary::StaticClass(), TempRooms);

	// const FVector2D CanvasCenter = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY()) / 2.f;
	const FVector2D CanvasCenter = CanvasPanel->GetDesiredSize() / 2.f;
	// GEngine->AddOnScreenDebugMessage(-1, 90.f, FColor::Red, FString::Printf(TEXT("Canvas Center: %s"), *CanvasCenter.ToString()));

	for (AActor* Room : TempRooms)
	{
		if (ACameraBoundary* Boundary = Cast<ACameraBoundary>(Room))
		{
			URoomTile* RoomTile = CreateWidget<URoomTile>(this, RoomTileClass);
			RoomTile->SetRoomState(ERoomState::Undiscovered);
			RoomTile->SetRoomType(ERoomType::None);
			CanvasPanel->AddChild(RoomTile);
			Rooms.Emplace(!Boundary->GetLevelAreaName().IsNone() ? Boundary->GetLevelAreaName() : Boundary->GetTileMapName(), RoomTile);

			UCanvasPanelSlot* MapCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(RoomTile);
			MapCPS->SetSize(FVector2D(Boundary->GetRoomSize().X * RoomTileSize, Boundary->GetRoomSize().Y * RoomTileSize));
			MapCPS->SetPosition(FVector2D{
				Boundary->GetRoomPosition().X * RoomTileSize + CanvasCenter.X /* + Boundary->GetRoomSize().X * RoomTileSize / 2.f*/,
				- Boundary->GetRoomPosition().Y * RoomTileSize + CanvasCenter.Y /* + Boundary->GetRoomSize().Y * RoomTileSize / 2.f*/});

			UE_LOG(LogTemp, Warning, TEXT("Room created-> size: (w%d, h%d), px: %f / %f, coords: (x%d, y%d), pos: %f / %f,"),
				Boundary->GetRoomSize().X, Boundary->GetRoomSize().Y,
				Boundary->GetRoomSize().X * RoomTileSize, Boundary->GetRoomSize().Y * RoomTileSize,
				Boundary->GetRoomPosition().X, Boundary->GetRoomPosition().Y,
				Boundary->GetRoomPosition().X * RoomTileSize, - Boundary->GetRoomPosition().Y * RoomTileSize);
		}
	}
}

void UMapWidget::UpdateRoomTile(const FMapUpdateData& Data)
{
	URoomTile* RoomTile = *Rooms.Find(Data.RoomName);
	if (!IsValid(RoomTile)) return;
	
	switch (Data.UpdateType) {
	case EMapUpdateType::None:
		break;
	case EMapUpdateType::PlayerEntering:
		RoomTile->SetDiscoveredTexture();
		RoomTile->HandleOccupiedAnimation(true);
		RoomTile->RevealRoom(Data.RoomType);
		RoomTile->SetRoomState(ERoomState::Discovered);
		break;
	case EMapUpdateType::PlayerLeaving:
		RoomTile->HandleOccupiedAnimation(false);
		break;
	case EMapUpdateType::PlayerDiscovering:
		break;
	}
}
