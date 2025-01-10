// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteActor.h"
#include "Interaction/InteractionInterface.h"
#include "Inventory/InventoryComponent.h"
#include "Item.generated.h"

class UGameplayAbility;
class UPaperSprite;
class USphereComponent;

UCLASS()
class LEYR_API AItem : public APaperSpriteActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AItem();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void Interact_Implementation(AActor* InteractingActor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventoryItemData ItemData;

	UFUNCTION()
	TArray<FGameplayTag> GetAbilities() { return Abilities; }
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> Abilities;

private:
	UPROPERTY() TObjectPtr<AActor> OverlappingActor;
};
