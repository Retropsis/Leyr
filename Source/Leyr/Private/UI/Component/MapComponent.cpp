// @ Retropsis 2024-2025.

#include "UI/Component/MapComponent.h"
#include "Blueprint/UserWidget.h"
#include "Data/MapData.h"
#include "Game/BaseGameplayTags.h"
#include "GameFramework/Character.h"
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

	PlayerCharacter = OwningController->GetCharacter();

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
		if (ACameraBoundary* Boundary = Cast<ACameraBoundary>(TempRoom))
		{
			FRoomData Data {
				!Boundary->GetLevelAreaName().IsNone() ? Boundary->GetLevelAreaName() : Boundary->GetTileMapName(),
				FIntPoint{ Boundary->GetRoomCoordinates().X, Boundary->GetRoomCoordinates().Y },
				FIntPoint(Boundary->GetRoomSize().X, Boundary->GetRoomSize().Y),
				Boundary->GetRoomType(),
				Boundary->ConstructSubdivisions()
			};
			SubdivisionTotalCount += Data.Subdivisions.Num();

			if (Data.RoomName.IsNone()) continue;
			
			UE_LOG(LogTemp, Warning, TEXT("Room created-> Name: %s"), *Data.RoomName.ToString());
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
	
	const FRoomData RoomData = *Rooms.Find(RoomName);
	const FIntPoint RoomCoordinates = RoomData.RoomCoordinates;
	// ExploreRoomTile(RoomName, PlayerCoordinates);
	UpdateRoomAt(RoomName, NewState, PlayerCoordinates);
	MapWidget->EnteringRoom(RoomData, NewState, PlayerCoordinates);
	
	StartTrackingPlayerRoomCoordinates(RoomName, RoomCoordinates);
}

void UMapComponent::StartTrackingPlayerRoomCoordinates(const FName& RoomName, const FIntPoint RoomCoordinates)
{
	GetWorld()->GetTimerManager().ClearTimer(TrackingPlayerTimer);
	TrackingPlayerTimer.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(TrackingPlayerTimer, [this,  RoomName, RoomCoordinates] ()
	{
		TrackPlayerRoomCoordinates(RoomName, RoomCoordinates);
	}, 1.f, true);
}

void UMapComponent::LeavingRoom(const FName& RoomName, const FIntPoint& PlayerCoordinates)
{
	if (!Rooms.Contains(RoomName)) return;
	
	MapWidget->LeavingRoom(*Rooms.Find(RoomName), PlayerCoordinates);
}

void UMapComponent::TrackPlayerRoomCoordinates(const FName& RoomName, const FIntPoint& RoomCoordinates)
{
	const FIntPoint PlayerCoordinates = GetPlayerRoomCoordinates(RoomCoordinates);
	UpdateRoomAt(RoomName, ERoomUpdateType::Entering, PlayerCoordinates);
}

FIntPoint UMapComponent::GetPlayerRoomCoordinates(const FIntPoint& RoomCoordinates) const
{
	if (!PlayerCharacter.IsValid()) return FIntPoint();
	
	const FIntPoint PlayerWorldCoordinates{ FMath::TruncToInt32(PlayerCharacter->GetActorLocation().X / 1280.f), FMath::TruncToInt32( PlayerCharacter->GetActorLocation().Z / 768.f) };
	return FIntPoint{ FMath::Abs(PlayerWorldCoordinates.X - RoomCoordinates.X), FMath::Abs(PlayerWorldCoordinates.Y - RoomCoordinates.Y)  };
}

void UMapComponent::UpdateRoomAt(const FName& RoomName, const ERoomUpdateType& UpdateType, const FIntPoint& PlayerCoordinates)
{
	if (!Rooms.Contains(RoomName)) return;
	
	const ERoomUpdateType NewState = IsRoomTileExplored(RoomName, PlayerCoordinates) ? UpdateType : ERoomUpdateType::Exploring;
	ExploreRoomTile(RoomName, PlayerCoordinates);
	MapWidget->UpdateRoomTileAt(*Rooms.Find(RoomName), NewState, PlayerCoordinates);
}

/*
 * TODO: Marked for refacto
 */
void UMapComponent::RevealHiddenWall(const FName& RoomName, const FIntPoint& RoomCoordinates, const ESubdivisionSide Side)
{
	if (Rooms.Find(RoomName)->Subdivisions.Contains(RoomCoordinates) && Rooms.Find(RoomName)->Subdivisions.Find(RoomCoordinates)->Doors.Contains(Side))
	{
		*Rooms.Find(RoomName)->Subdivisions.Find(RoomCoordinates)->Doors.Find(Side) = EEntranceType::None;
		MapWidget->UpdateRoomTileAt(*Rooms.Find(RoomName), ERoomUpdateType::RevealDoor, RoomCoordinates);
	}
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
		SubdivisionExploredCount++;
		MapWidget->UpdateCompletionText(static_cast<float>(SubdivisionExploredCount) / static_cast<float>(SubdivisionTotalCount) * 100.f);
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

void UMapComponent::UnveilRooms(const TArray<FName>& RoomNames)
{
	for (int i = 0; i < RoomNames.Num(); ++i)
	{
		FName RoomName = RoomNames[i];
		UnveilRoom(RoomName);
		if (Rooms.Contains(RoomName))
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, RoomName] ()
			{
				MapWidget->UnveilRoom(*Rooms.Find(RoomName));
			}, (i * UnveilRoomAnimationDelay) + UnveilRoomAnimationFirstDelay, false);
		}
	}
}
