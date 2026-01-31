// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Data/MapData.h"
#include "RoomTile.generated.h"

struct FMinimapData;
enum class ERoomState : uint8;
class UImage;

UCLASS()
class LEYR_API URoomTile : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:

	void UpdateRoomTile(const ERoomUpdateType& UpdateType);
	void InitializeRoomTile(const FRoomData& RoomData, const FIntPoint& SubdivisionCoordinates, const FMinimapData& MinimapData);
	void EnterRoomTile();
	void LeaveRoomTile();
	void ExploreRoomTile();
	void SetRoomTileBrushByType(ERoomType Type) const;
	void UnveilRoomTile();
	void RevealRoomTile();
	void DrawDoors(const FSubdivision& Subdivision) const;
	
	void SetBorders(const FIntPoint& Coordinates, const FIntPoint& Size) const;
	void SetBordersUnexplored(const FIntPoint& Coordinates, const FIntPoint& Size) const;

	void SetRoomState(const ERoomState NewState) { RoomState = NewState; }
	ERoomState GetRoomState() const { return RoomState; }
	void SetRoomType(const ERoomType NewType) { RoomType = NewType; }
	ERoomType GetRoomType() const { return RoomType; }
	void SetOriginalPositionInCanvas(const FVector2D Position) { OriginalPositionInCanvas = Position; }
	FVector2D GetOriginalPositionInCanvas() const { return OriginalPositionInCanvas; }
	void SetRoomSize(const FIntPoint& Size) { RoomSize = Size; }
	void SetTileCoordinates(const FIntPoint& Coordinates) { TileCoordinates = Coordinates; }

	UFUNCTION(BlueprintImplementableEvent)
	void HandleOccupiedAnimation(bool bIsOccupied);

	UFUNCTION(BlueprintImplementableEvent)
	void HandleRevealAnimation();

protected:
	void InitializeTint(const FMinimapData& Data);

private:
	UPROPERTY(EditDefaultsOnly, Category ="Map")
	float BorderSize = 4.f;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Room;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_BorderTop;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_BorderLeft;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_BorderRight;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_BorderBottom;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_CornerTopLeft;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_CornerTopRight;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_CornerBottomLeft;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_CornerBottomRight;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Item;

	/*
	 * Tints
	 */
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateColor ExploredTint;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateColor WallTint;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateColor DoorTint;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateColor CornerTint;
	
	/*
	 * Rooms
	 */
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Unexplored;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Explored;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Occupied;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_SaveRoom;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_WarpRoom;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_BossRoom;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_ExitRoom;

	/*
	 * Unexplored
	 */
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Border_Undiscovered_H;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Border_Undiscovered_V;

	/*
	 * Walls
	 */
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Horizontal_Wall_Top;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Horizontal_Wall_Bottom;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Vertical_Wall_Left;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Vertical_Wall_Right;

	/*
	 * Corners
	 */
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Corner_Top_Left;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Corner_Top_Right;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Corner_Bottom_Left;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Corner_Bottom_Right;

	/*
	 * Doors
	 */
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Horizontal_Door_Top;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Horizontal_Door_Bottom;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Vertical_Door_Left;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Vertical_Door_Right;

	FIntPoint RoomSize;
	FIntPoint TileCoordinates;
	ERoomState RoomState = ERoomState::Hidden;
	ERoomType RoomType = ERoomType::None;
	FVector2D OriginalPositionInCanvas = FVector2D::ZeroVector;
};
