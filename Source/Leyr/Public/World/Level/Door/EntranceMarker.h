// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Components/BillboardComponent.h"
#include "Components/SceneComponent.h"
#include "Data/MapData.h"
#include "EntranceMarker.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class LEYR_API UEntranceMarker : public UBillboardComponent
{
	GENERATED_BODY()

public:
	EEntranceType GetEntranceType() const { return EntranceType; }
	void SetEntranceType(const EEntranceType Type) { EntranceType = Type; }

private:
	UPROPERTY(EditAnywhere, Category="Boundary")
	EEntranceType EntranceType = EEntranceType::None;
};
