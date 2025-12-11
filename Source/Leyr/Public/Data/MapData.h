#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/RendererSettings.h"
#include "Game/BaseGameplayTags.h"
#include "MapData.generated.h"

UENUM()
enum class ERoomUpdateType : uint8
{
	None,
	PlayerEntering,
	PlayerLeaving,
	PlayerDiscovering,
};

UENUM()
enum class ERoomState : uint8
{
	None,
	Undiscovered,
	Discovered,
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

USTRUCT()
struct FRoomData
{
	GENERATED_BODY()
	FRoomData() {}
	FRoomData(const FName& Name, const FIntPoint& Coordinates,  const FIntPoint& Size, const ERoomType Type) :
		RoomName(Name), RoomCoordinates(Coordinates), RoomSize(Size), RoomType(Type) {}

	UPROPERTY() FGameplayTag RegionTag = FBaseGameplayTags::Get().Map_Region_Dorn;
	UPROPERTY() FName RoomName = FName();
	UPROPERTY() FIntPoint RoomCoordinates = FIntPoint();
	UPROPERTY() FIntPoint RoomSize = FIntPoint();
	UPROPERTY() ERoomType RoomType = ERoomType::None;
	UPROPERTY() bool bWasDiscovered = false;
};

inline bool operator==(const FRoomData& A, FRoomData& B)
{
	return A.RoomName == B.RoomName;
}