// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MapWidget.generated.h"

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
	void StartInterpolation(const ERoomUpdateType& UpdateType, const FVector2D& RoomTilePosition);
	void EnteringRoom(const FRoomData& RoomData, const ERoomUpdateType& UpdateType, const FIntPoint& PlayerCoordinates);
	void LeavingRoom(const FRoomData& RoomData, const FIntPoint& PlayerCoordinates);
	void UpdateRoomTileAt(const FRoomData& RoomData, const ERoomUpdateType& UpdateType, const FIntPoint& Coordinates);

	UFUNCTION(BlueprintCallable)
	void RedrawMap(float DeltaSecond);
	
private:
	UPROPERTY(EditDefaultsOnly)
	float RoomTileSize = 32.f;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, URoom*> Rooms;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URoom> RoomClass = nullptr;
	FVector2D CanvasCenter;
	FVector2D CurrentRoomOffset = FVector2D::ZeroVector;
	FVector2D TargetRoomOffset = FVector2D::ZeroVector;
	bool bShouldInterpolate = false;
};
