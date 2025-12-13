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

void UMapComponent::EnteringRoom(const FName& RoomName, const ERoomUpdateType& UpdateType, const FIntPoint& PlayerCoordinates)
{
	if (!Rooms.Contains(RoomName)) return;
	
	const ERoomUpdateType NewState = !IsRoomUnveiled(RoomName) ? ERoomUpdateType::Unveiling : UpdateType;
	UnveilRoom(RoomName);
	MapWidget->EnteringRoom(*Rooms.Find(RoomName), NewState, PlayerCoordinates);
	ExploreRoomTile(RoomName, PlayerCoordinates);
}

void UMapComponent::LeavingRoom(const FName& RoomName, const FIntPoint& PlayerCoordinates)
{
	if (!Rooms.Contains(RoomName)) return;
	
	MapWidget->LeavingRoom(*Rooms.Find(RoomName), PlayerCoordinates);
}

void UMapComponent::UpdateRoomAt(const FName& RoomName, const ERoomUpdateType& UpdateType, const FIntPoint& Coordinates)
{
	if (!Rooms.Contains(RoomName)) return;
	
	const ERoomUpdateType NewState = IsRoomTileExplored(RoomName, Coordinates) ? UpdateType : ERoomUpdateType::Exploring;
	ExploreRoomTile(RoomName, Coordinates);
	MapWidget->UpdateRoomTileAt(*Rooms.Find(RoomName), NewState, Coordinates);
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

bool UMapComponent::IsRoomUnveiled(const FName& RoomName) const
{
	return Rooms.Find(RoomName)->bWasUnveiled;
}

void UMapComponent::SetRoomUnveiled(const FName& RoomName, const bool bUnveiled)
{
	Rooms.Find(RoomName)->bWasUnveiled = bUnveiled;
}

bool UMapComponent::IsRoomTileExplored(const FName& RoomName, const FIntPoint& Coordinates) const
{
	if (Rooms.Find(RoomName)->Subdivisions.Contains(Coordinates))
	{
		return Rooms.Find(RoomName)->Subdivisions.Find(Coordinates)->SubdivisionState == ESubdivisionState::Explored;
	}
	return false;
}

void UMapComponent::SetRoomTileExplored(const FName& RoomName, const FIntPoint& Coordinates)
{
	if (Rooms.Find(RoomName)->Subdivisions.Contains(Coordinates))
	{
		Rooms.Find(RoomName)->Subdivisions.Find(Coordinates)->SubdivisionState = ESubdivisionState::Explored;
	}
}

void UMapComponent::ExploreRoomTile(const FName& RoomName, const FIntPoint& Coordinates)
{
	if (!IsRoomTileExplored(RoomName, Coordinates))
	{
		SetRoomTileExplored(RoomName, Coordinates);
	}
}

void UMapComponent::UnveilRoom(const FName& RoomName)
{
	if (Rooms.Contains(RoomName))
	{
		Rooms.Find(RoomName)->bWasUnveiled = true;
		for (TTuple<FIntPoint, FSubdivision>& Subdivision : Rooms.Find(RoomName)->Subdivisions)
		{
			if (Subdivision.Value.SubdivisionState == ESubdivisionState::Hidden)
			{
				Subdivision.Value.SubdivisionState = ESubdivisionState::Unexplored;
			}
		}
	}
}
