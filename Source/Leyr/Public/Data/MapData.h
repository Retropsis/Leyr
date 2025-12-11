#pragma once

#include "CoreMinimal.h"
#include "MapData.generated.h"

UENUM()
enum class EMapUpdateType : uint8
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
	SaveRoom,
	BossRoom,
};

UENUM()
enum class ERoomType : uint8
{
	None,
	SaveRoom,
	WarpRoom,
	BossRoom,
};

USTRUCT()
struct FMapUpdateData
{
	GENERATED_BODY()

	UPROPERTY() FName RoomName = FName();
	UPROPERTY() FIntPoint RoomCoordinates = FIntPoint();
	UPROPERTY() EMapUpdateType UpdateType = EMapUpdateType::None;
	UPROPERTY() ERoomType RoomType = ERoomType::None;
};