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
	void StartTrackingPlayerRoomCoordinates(const FName& RoomName, FIntPoint RoomCoordinates);
	void EnteringRoom(const FName& RoomName, const ERoomUpdateType& UpdateType, const FIntPoint& PlayerCoordinates);
	void LeavingRoom(const FName& RoomName, const FIntPoint& PlayerCoordinates);
	void UpdateRoomAt(const FName& RoomName, const ERoomUpdateType& UpdateType, const FIntPoint& PlayerCoordinates);
	void RevealHiddenWall(const FName& RoomName, const FIntPoint& RoomCoordinates, ESubdivisionSide Side);
	void UnveilRooms(const TArray<FName>& RoomNames);
	
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
	void TrackPlayerRoomCoordinates(const FName& RoomName, const FIntPoint& RoomCoordinates);
	FIntPoint GetPlayerRoomCoordinates(const FIntPoint& RoomCoordinates) const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSubclassOf<UMapWidget> MapWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	float UnveilRoomAnimationFirstDelay = .05f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	float UnveilRoomAnimationDelay = .1f;

	UPROPERTY()
	TObjectPtr<UMapWidget> MapWidget;

	UPROPERTY()
	TMap<FName, FRoomData> Rooms;

	UPROPERTY() uint32 SubdivisionExploredCount = 0;
	UPROPERTY() uint32 SubdivisionTotalCount = 0;
	
	TWeakObjectPtr<APlayerController> OwningController;
	TWeakObjectPtr<ACharacter> PlayerCharacter;
	FTimerHandle TrackingPlayerTimer;
};
