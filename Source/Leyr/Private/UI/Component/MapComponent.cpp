// @ Retropsis 2024-2025.

#include "UI/Component/MapComponent.h"
#include "Blueprint/UserWidget.h"
#include "Data/MapData.h"
#include "Game/BaseGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Map/MapWidget.h"
#include "World/Map/CameraBoundary.h"

UMapComponent::UMapComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMapComponent::InitializeMap()
{
	ConstructMapRooms();
	ConstructMapWidget();
}

void UMapComponent::ConstructMapWidget()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("Map Component should have PlayerController as Owner."));
	if (!OwningController->IsLocalController()) return;

	MapWidget = CreateWidget<UMapWidget>(OwningController.Get(), MapWidgetClass);
	MapWidget->ConstructMapCanvas(FilterRoomsByRegion(FBaseGameplayTags::Get().Map_Region_Dorn));
	// CloseInventoryMenu();
}

/*
 * Find all CameraBoundary actors in the region and construct data
 * This might be better the other way around where the CameraBoundary announces itself to the MapComponent
 */
void UMapComponent::ConstructMapRooms()
{
	TArray<AActor*> TempRooms;
	UGameplayStatics::GetAllActorsOfClass(this, ACameraBoundary::StaticClass(), TempRooms);
	Rooms.Empty();
	for (AActor* TempRoom : TempRooms)
	{
		if (const ACameraBoundary* Boundary = Cast<ACameraBoundary>(TempRoom))
		{
			FRoomData Data {
				!Boundary->GetLevelAreaName().IsNone() ? Boundary->GetLevelAreaName() : Boundary->GetTileMapName(),
				FIntPoint{ Boundary->GetRoomCoordinates().X, Boundary->GetRoomCoordinates().Y },
				FIntPoint(Boundary->GetRoomSize().X, Boundary->GetRoomSize().Y),
				Boundary->GetRoomType()
			};

			if (Data.RoomName.IsNone()) continue;
			
			// UE_LOG(LogTemp, Warning, TEXT("Room created-> Name: %s size: (w%d, h%d), coords: (x%d, y%d) Type: %s"), *Data.RoomName.ToString(), Data.RoomSize.X, Data.RoomSize.Y, Data.RoomCoordinates.X, Data.RoomCoordinates.Y, *UEnum::GetValueAsString(Data.RoomType));
			
			Rooms.Add(Data.RoomName, Data);
		}
	}
}

void UMapComponent::UpdateRoom(const FName& RoomName, ERoomUpdateType& UpdateType) const
{
	if (UpdateType == ERoomUpdateType::PlayerEntering)
	{
		UpdateType =  !IsRoomDiscovered(RoomName) ? ERoomUpdateType::PlayerDiscovering : UpdateType;
	}
	MapWidget->UpdateRoomTile(RoomName, UpdateType);
}

TArray<FRoomData> UMapComponent::FilterRoomsByRegion(const FGameplayTag& RegionTag)
{
	TArray<FRoomData> RegionRooms;
	for (const TTuple<FName, FRoomData>& Room : Rooms)
	{
		if (Room.Value.RegionTag.MatchesTagExact(RegionTag))
		{
			RegionRooms.Add(Room.Value);
		}
	}
	return RegionRooms;
}

FRoomData UMapComponent::FindRoomByName(const FName& RoomName) const
{
	return Rooms.Contains(RoomName) ? *Rooms.Find(RoomName) : FRoomData();
}

bool UMapComponent::IsRoomDiscovered(const FName& RoomName) const
{
	return Rooms.Contains(RoomName) ? Rooms.Find(RoomName)->bWasDiscovered : false;
}

void UMapComponent::SetRoomDiscovered(const FName& RoomName, const bool bDiscovered)
{
	if (Rooms.Contains(RoomName)) Rooms.Find(RoomName)->bWasDiscovered = bDiscovered;
}
