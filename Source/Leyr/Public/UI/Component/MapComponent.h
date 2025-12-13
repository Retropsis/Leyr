// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Data/MapData.h"
#include "MapComponent.generated.h"

class UMapWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LEYR_API UMapComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMapComponent();
	void ConstructMapWidget();
	void ConstructMapRooms();
	void EnteringRoom(const FName& RoomName, const ERoomUpdateType& UpdateType, const FIntPoint& PlayerCoordinates);
	void LeavingRoom(const FName& RoomName, const FIntPoint& PlayerCoordinates);
	void UpdateRoomAt(const FName& RoomName, const ERoomUpdateType& UpdateType, const FIntPoint& Coordinates);
	
	UFUNCTION(BlueprintCallable, Category="Map")
	UMapWidget* GetMapWidget() const { return MapWidget; }
	
	UFUNCTION(BlueprintCallable, Category="Map")
	void InitializeMap();

private:
	TArray<FRoomData> FilterRoomsByRegion(const FGameplayTag& RegionTag);
	FRoomData FindRoomByName(const FName& RoomName) const;
	bool IsRoomUnveiled(const FName& RoomName) const;
	void SetRoomUnveiled(const FName& RoomName, bool bUnveiled = true);
	bool IsRoomTileExplored(const FName& RoomName, const FIntPoint& Coordinates) const;
	void SetRoomTileExplored(const FName& RoomName, const FIntPoint& Coordinates);
	void ExploreRoomTile(const FName& RoomName, const FIntPoint& Coordinates);
	void UnveilRoom(const FName& RoomName);
	
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSubclassOf<UMapWidget> MapWidgetClass;

	UPROPERTY()
	TObjectPtr<UMapWidget> MapWidget;

	UPROPERTY()
	TMap<FName, FRoomData> Rooms;
	
	TWeakObjectPtr<APlayerController> OwningController;
};
