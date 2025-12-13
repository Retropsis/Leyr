// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Data/MapData.h"
#include "Room.generated.h"

class URoomTile;
class UGridPanel;

UCLASS()
class LEYR_API URoom : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void ConstructRoom(const FRoomData& RoomData);
	void EnteringRoom(const FIntPoint& PlayerCoordinates, const FRoomData& RoomData);
	void UnveilingRoom(const FIntPoint& PlayerCoordinates, const FRoomData& RoomData);
	void LeavingRoom(const FIntPoint& PlayerCoordinates, const FRoomData& RoomData);
	void UpdateRoomTile(const ERoomUpdateType& UpdateType, const FIntPoint& Coordinates, const FRoomData& RoomData);
	void LeaveRoomTile(URoomTile* RoomTile);
	
	void SetRoomState(const ERoomState NewState) { RoomState = NewState; }
	ERoomState GetRoomState() const { return RoomState; }
	void SetRoomType(const ERoomType NewType) { RoomType = NewType; }
	ERoomType GetRoomType() const { return RoomType; }
	void SetOriginalPositionInCanvas(const FVector2D Position) { OriginalPositionInCanvas = Position; }
	FVector2D GetOriginalPositionInCanvas() const { return OriginalPositionInCanvas; }
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UGridPanel> RoomGrid;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URoomTile> RoomTileClass = nullptr;
	
	UPROPERTY()
	TMap<FIntPoint, URoomTile*> RoomTiles;

	FIntPoint RoomSize;
	TWeakObjectPtr<URoomTile> LastOccupiedRoomTile;
	ERoomState RoomState = ERoomState::None;
	ERoomType RoomType = ERoomType::None;
	FVector2D OriginalPositionInCanvas = FVector2D::ZeroVector;
};
