// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PaperFlipbookActor.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "Inventory/InventoryComponent.h"
#include "Item.generated.h"

class UGameplayAbility;
class UPaperSprite;
class USphereComponent;

UCLASS()
class LEYR_API AItem : public APaperFlipbookActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AItem();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void Interact_Implementation(AActor* InteractingActor) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FInventoryItemData ItemData;
	
	int32 ID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FGameplayTag> Abilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundBase> PickupSound = nullptr;

	UFUNCTION()
	TArray<FGameplayTag> GetAbilities() { return Abilities; }
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> Sphere;

private:
	UPROPERTY() TObjectPtr<AActor> OverlappingActor;
};
