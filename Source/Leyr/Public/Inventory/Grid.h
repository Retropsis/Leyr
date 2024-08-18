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

	UPROPERTY(BlueprintReadWrite) int32 X = 0;
	UPROPERTY(BlueprintReadWrite) int32 Y = 0;
};