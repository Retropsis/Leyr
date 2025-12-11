// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MapComponent.generated.h"

class UMapWidget;
struct FMapUpdateData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LEYR_API UMapComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMapComponent();
	void UpdateMap(const FMapUpdateData& Data) const;
	virtual void BeginPlay() override;
	void ConstructMap();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSubclassOf<UMapWidget> MapWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TObjectPtr<UMapWidget> MapWidget;
	
	TWeakObjectPtr<APlayerController> OwningController;
};
