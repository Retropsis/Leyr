// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Data/MapData.h"
#include "World/Data/MapInfo.h"
#include "MapComponent.generated.h"

class UMapWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LEYR_API UMapComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMapComponent();
	void InitializeExplorationData();
	void ConstructMapWidget();
	void ConstructMapRooms();
	void StartTrackingPlayerRoomCoordinates(const FName& RoomName, const FVector& RoomLocation);
	void EnteringRoom(const FName& RoomName, const ERoomUpdateType& UpdateType, const FIntPoint& PlayerCoordinates);
	void LeavingRoom(const FName& RoomName, const FIntPoint& PlayerCoordinates);
	void UpdateRoomAt(const FName& RoomName, const ERoomUpdateType& UpdateType, const FIntPoint& PlayerCoordinates);
	void RevealHiddenWall(const FName& RoomName, const FIntPoint& RoomCoordinates, ESubdivisionSide Side);
	void UnveilRooms(const TArray<FName>& RoomNames);
	
	UFUNCTION(BlueprintCallable, Category="Map")
	UMapWidget* GetMapWidget() const { return MapWidget; }
	
	UFUNCTION(BlueprintCallable, Category="Map")
	void InitializeMap();
	FGameplayTag GetCurrentRegionTag() const;

	TMap<FName, FRoomData> GetRooms() const { return Rooms; }
	void SetRooms(const TMap<FName, FRoomData>& InRooms) { Rooms = InRooms; }
	float GetMapCompletionRate() const { return TotalCompletionRate; }

private:
	TArray<FRoomData> FilterRoomsByRegion(const FGameplayTag& RegionTag);
	FRoomData FindRoomByName(const FName& RoomName) const;
	bool IsRoomUnveiled(const FName& RoomName) const;
	void SetRoomUnveiled(const FName& RoomName, bool bUnveiled = true);
	bool IsRoomTileExplored(const FName& RoomName, const FIntPoint& Coordinates) const;
	void SetRoomTileExplored(const FName& RoomName, const FIntPoint& Coordinates);
	void UpdateCompletionRate();
	void ExploreRoomTile(const FName& RoomName, const FIntPoint& Coordinates);
	void UnveilRoom(const FName& RoomName);
	void TrackPlayerRoomCoordinates(const FName& RoomName, const FVector& RoomLocation);
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

	UPROPERTY() TMap<FGameplayTag, FExplorationData> ExplorationData;
	uint32 SubdivisionTotalCount = 0;
	float TotalCompletionRate = 0.f;
	
	TWeakObjectPtr<APlayerController> OwningController;
	TWeakObjectPtr<ACharacter> PlayerCharacter;
	FTimerHandle TrackingPlayerTimer;
};
