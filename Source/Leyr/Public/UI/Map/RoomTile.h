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
	void RevealRoom() const;

	void SetRoomState(const ERoomState NewState) { RoomState = NewState; }
	ERoomState GetRoomState() const { return RoomState; }
	void SetRoomType(const ERoomType NewType) { RoomType = NewType; }
	ERoomType GetRoomType() const { return RoomType; }
	void SetOriginalPositionInCanvas(const FVector2D Position) { OriginalPositionInCanvas = Position; }
	FVector2D GetOriginalPositionInCanvas() const { return OriginalPositionInCanvas; }

	UFUNCTION(BlueprintImplementableEvent)
	void HandleOccupiedAnimation(bool bIsOccupied);

	UFUNCTION(BlueprintImplementableEvent)
	void HandleRevealAnimation();

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Room;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Undiscovered;
	
	UPROPERTY(EditAnywhere, Category="Map")
	FSlateBrush Brush_Discovered;
	
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

	ERoomState RoomState = ERoomState::None;
	ERoomType RoomType = ERoomType::None;
	FVector2D OriginalPositionInCanvas = FVector2D::ZeroVector;
};
