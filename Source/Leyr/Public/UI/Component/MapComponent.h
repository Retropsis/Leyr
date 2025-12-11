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
	void UpdateRoom(const FName& RoomName, ERoomUpdateType& UpdateType) const;
	
	UFUNCTION(BlueprintCallable, Category="Map")
	UMapWidget* GetMapWidget() const { return MapWidget; }
	
	UFUNCTION(BlueprintCallable, Category="Map")
	void InitializeMap();

private:
	TArray<FRoomData> FilterRoomsByRegion(const FGameplayTag& RegionTag);
	FRoomData FindRoomByName(const FName& RoomName) const;
	bool IsRoomDiscovered(const FName& RoomName) const;
	void SetRoomDiscovered(const FName& RoomName, bool bDiscovered = true);
	
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSubclassOf<UMapWidget> MapWidgetClass;

	UPROPERTY()
	TObjectPtr<UMapWidget> MapWidget;

	UPROPERTY()
	TMap<FName, FRoomData> Rooms;
	
	TWeakObjectPtr<APlayerController> OwningController;
};
