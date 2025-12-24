// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Components/BillboardComponent.h"
#include "EncounterSpawnPointComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class LEYR_API UEncounterSpawnPointComponent : public UBillboardComponent
{
	GENERATED_BODY()

public:
	UEncounterSpawnPointComponent();
};
