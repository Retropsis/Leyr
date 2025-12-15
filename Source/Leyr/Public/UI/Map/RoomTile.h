// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Data/MapData.h"
#include "RoomTile.generated.h"

enum class ERoomState : uint8;
class UImage;

UCLASS()
class LEYR_API URoomTile : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	// virtual void NativeOnInitialized() override;

	void UpdateRoomTile(const ERoomUpdateType& UpdateType);
	
	void EnterRoomTile();
	void LeaveRoomTile();
	void ExploreRoomTile();
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
	void SetRoomCoordinates(const FIntPoint& Coordinates) { RoomCoordinates = Coordinates; }

	UFUNCTION(BlueprintImplementableEvent)
	void HandleOccupiedAnimation(bool bIsOccupied);

	UFUNCTION(BlueprintImplementableEvent)
	void HandleRevealAnimation();

private:
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
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Border_Undiscovered_H;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Border_Undiscovered_V;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Border_H;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Border_V;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_DoorBorder_H;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_DoorBorder_V;

	FIntPoint RoomSize;
	FIntPoint RoomCoordinates;
	ERoomState RoomState = ERoomState::Hidden;
	ERoomType RoomType = ERoomType::None;
	FVector2D OriginalPositionInCanvas = FVector2D::ZeroVector;
};
