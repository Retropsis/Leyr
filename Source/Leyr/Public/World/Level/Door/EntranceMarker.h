// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Components/BillboardComponent.h"
#include "Components/SceneComponent.h"
#include "EntranceMarker.generated.h"

class AEntrance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class LEYR_API UEntranceMarker : public UBillboardComponent
{
	GENERATED_BODY()
};
