#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Game/BaseGameplayTags.h"
#include "MapData.generated.h"

UENUM()
enum class ERoomUpdateType : uint8
{
	None,
	Entering,
	Leaving,
	Unveiling,
	Exploring,
};

UENUM()
enum class ESubdivisionState : uint8
{
	Hidden,
	Unexplored,
	Explored,
};

UENUM()
enum class ERoomState : uint8
{
	None,
	Hidden,
	Unexplored,
	Explored,
	Occupied,
};

UENUM()
enum class ERoomType : uint8
{
	None,
	SaveRoom,
	WarpRoom,
	BossRoom,
	ExitRoom,
};

UENUM()
enum class EDoorPlacement : uint8
{
	None,
	Top,
	Bottom,
	Left,
	Right,
};

USTRUCT()
struct FSubdivision
{
	GENERATED_BODY()

public:
	UPROPERTY()
	ESubdivisionState SubdivisionState = ESubdivisionState::Hidden;
	
	UPROPERTY()
	TSet<EDoorPlacement> Doors; 
};

USTRUCT()
struct FRoomData
{
	GENERATED_BODY()
	FRoomData() {}
	FRoomData(const FName& Name, const FIntPoint& Coordinates,  const FIntPoint& Size, const ERoomType Type, const TMap<FIntPoint, FSubdivision>& Divisions) :
		RoomName(Name), RoomCoordinates(Coordinates), RoomSize(Size), RoomType(Type), Subdivisions(Divisions)
	{
		// for (int h = 0; h < RoomSize.Y; ++h)
		// {
		// 	for (int w = 0; w < RoomSize.X; ++w)
		// 	{
		// 		Subdivisions.Emplace(FIntPoint(w, h), FSubdivision{} );
		// 		// UE_LOG(LogTemp, Warning, TEXT("Constructing subdivisions for room name: %s, at room coords x:%d, y:%d"), *RoomName.ToString(), w, h);
		// 	}
		// }
	}

	UPROPERTY() FGameplayTag RegionTag = FBaseGameplayTags::Get().Map_Region_Dorn;
	UPROPERTY() FName RoomName = FName();
	UPROPERTY() FIntPoint RoomCoordinates = FIntPoint();
	UPROPERTY() FIntPoint RoomSize = FIntPoint();
	UPROPERTY() ERoomType RoomType = ERoomType::None;
	UPROPERTY() bool bWasUnveiled = false;
	UPROPERTY() TMap<FIntPoint, FSubdivision> Subdivisions;
};

inline bool operator==(const FRoomData& A, const FRoomData& B)
{
	return A.RoomName == B.RoomName;
}