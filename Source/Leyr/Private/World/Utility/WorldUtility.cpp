// @ Retropsis 2024-2025.

#include "World/Utility/WorldUtility.h"
#include "Data/MapData.h"

float UWorldUtility::GetAngleBetweenPoints(const FVector& Point_A, const FVector& Point_B)
{
	const FVector2D Point_A_2D = FVector2D{ Point_A.X, Point_A.Z };
	const FVector2D Point_B_2D = FVector2D{ Point_B.X, Point_B.Z };
	return FMath::RadiansToDegrees(FMath::Atan2((Point_A_2D - Point_B_2D).Y, (Point_A_2D - Point_B_2D).X));
}

ESubdivisionSide UWorldUtility::GetSubdivisionSideFromAngle(const float Angle)
{
	ESubdivisionSide Side = ESubdivisionSide::Right;
	if (Angle <= -45.f && Angle > -135.f)
	{
		Side = ESubdivisionSide::Top;
	}
	if (Angle <= 45.f && Angle > -45.f)
	{
		Side = ESubdivisionSide::Left;
	}
	if (Angle > 45.f && Angle < 135.f)
	{
		Side = ESubdivisionSide::Bottom;
	}
	return Side;
}

FIntPoint UWorldUtility::GetWorldCoordinates(const FVector& WorldLocation)
{
	const FIntPoint WorldCoordinates = FIntPoint{ FMath::FloorToInt32(WorldLocation.X / 1280.f), FMath::FloorToInt32(WorldLocation.Z / 768.f) };
	// GEngine->AddOnScreenDebugMessage(-1, 90.f, FColor::Cyan, FString::Printf(TEXT("WorldCoordinates: (x: %d, y: %d)"), WorldCoordinates.X, WorldCoordinates.Y));
	return WorldCoordinates;
}

FIntPoint UWorldUtility::GetPlayerRoomCoordinates(const FVector& PlayerLocation, const FVector& RoomLocation)
{
	const FVector PlayerRelativeLocation = PlayerLocation - RoomLocation;
	const FIntPoint PlayerRoomCoordinates = FIntPoint{ FMath::Abs(FMath::TruncToInt32(PlayerRelativeLocation.X / 1280.f)), FMath::Abs(FMath::TruncToInt32(PlayerRelativeLocation.Z / 768.f)) };
	// GEngine->AddOnScreenDebugMessage(-1, 90.f, FColor::Cyan, FString::Printf(TEXT("PlayerRoomCoordinates: (x: %d, y: %d)"), PlayerRoomCoordinates.X, PlayerRoomCoordinates.Y));
	return PlayerRoomCoordinates;
}

FIntPoint UWorldUtility::GetRoomCoordinates(const FVector& WorldLocation)
{
	const FIntPoint RoomCoordinates = FIntPoint{ FMath::FloorToInt32(WorldLocation.X / 1280.f), FMath::FloorToInt32(WorldLocation.Z / 768.f) };
	// GEngine->AddOnScreenDebugMessage(-1, 90.f, FColor::Cyan, FString::Printf(TEXT("RoomCoordinates: (x: %d, y: %d)"), RoomCoordinates.X, RoomCoordinates.Y));
	return RoomCoordinates;
}