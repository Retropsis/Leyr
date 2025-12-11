// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MapWidget.generated.h"

struct FMapUpdateData;
class URoomTile;
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
	virtual void NativeOnInitialized() override;
	void UpdateRoomTile(const FMapUpdateData& Data);
	
private:
	void ConstructMap();
	
	UPROPERTY(EditDefaultsOnly)
	float RoomTileSize = 32.f;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, URoomTile*> Rooms;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URoomTile> RoomTileClass = nullptr;
};
