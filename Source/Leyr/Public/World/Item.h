// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteActor.h"
#include "Inventory/InventoryComponent.h"
#include "Item.generated.h"

class UPaperSprite;
class USphereComponent;

UCLASS()
class LEYR_API AItem : public APaperSpriteActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventoryItemData ItemData;
};
