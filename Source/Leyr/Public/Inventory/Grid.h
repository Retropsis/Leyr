#pragma once

#include "CoreMinimal.h"
#include "Grid.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FLine
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite) FVector2D Start = FVector2D::ZeroVector;
	UPROPERTY(BlueprintReadWrite) FVector2D End = FVector2D::ZeroVector;
};

USTRUCT(BlueprintType, Blueprintable)
struct FTile
{
	GENERATED_BODY()
	
	bool operator== (const FTile& InTile)
	{
		return X == InTile.X && Y == InTile.Y;
	}
	
	uint32 GetTypeHash(const FTile& InTile)
	{
		return HashCombine(::GetTypeHash(InTile.X), ::GetTypeHash(InTile.Y));
	}
	
	UPROPERTY(BlueprintReadWrite) int32 X = 0;
	UPROPERTY(BlueprintReadWrite) int32 Y = 0;
};