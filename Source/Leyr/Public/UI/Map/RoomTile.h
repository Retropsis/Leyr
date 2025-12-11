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

	void SetUndiscoveredTexture();
	void SetDiscoveredTexture();
	void SetOccupiedTexture();
	void RevealRoom(ERoomType NewType) const;

	void SetRoomState(const ERoomState NewState) { RoomState = NewState; }
	ERoomState GetRoomState() const { return RoomState; }
	void SetRoomType(const ERoomType NewType) { RoomType = NewType; }
	ERoomType GetRoomType() const { return RoomType; }

	UFUNCTION(BlueprintImplementableEvent)
	void HandleOccupiedAnimation(bool bIsOccupied);

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

	ERoomState RoomState = ERoomState::None;
	ERoomType RoomType = ERoomType::None;
};
