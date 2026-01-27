// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MapWidget.generated.h"

class UTextBlock;
enum class ERoomUpdateType : uint8;
struct FRoomData;
class URoom;
class UImage;
class ACameraBoundary;
class UCanvasPanel;

/**
 * 
 */
UCLASS()
class LEYR_API UMapWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void ConstructMapCanvas(const TArray<FRoomData>& MapData);
	void InterpolateMinimapToCenter(const ERoomUpdateType& UpdateType, URoom* Room);
	void StartInterpolation(const FVector2D& RoomTilePosition);
	void EnteringRoom(const FRoomData& RoomData, const ERoomUpdateType& UpdateType, const FIntPoint& PlayerCoordinates);
	void LeavingRoom(const FRoomData& RoomData, const FIntPoint& PlayerCoordinates);
	void UnveilRoom(const FRoomData& RoomData);
	void UpdateRoomTileAt(const FRoomData& RoomData, const ERoomUpdateType& UpdateType, const FIntPoint& PlayerCoordinates);
	void UpdateCompletionText(float CompletionRate, float RegionRate) const;

	UFUNCTION(BlueprintCallable)
	void RedrawMap(float DeltaSecond);
	
private:
	UPROPERTY(EditDefaultsOnly)
	float RoomTileSize_X = 32.f;
	
	UPROPERTY(EditDefaultsOnly)
	float RoomTileSize_Y = 32.f;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_RoomName;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_MapCompletion;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_RegionCompletion;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, URoom*> Rooms;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URoom> RoomClass = nullptr;
	FVector2D CanvasCenter;
	FVector2D CurrentRoomOffset = FVector2D::ZeroVector;
	FVector2D TargetRoomOffset = FVector2D::ZeroVector;
	bool bShouldInterpolate = false;
};
