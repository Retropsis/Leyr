// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PaperFlipbookActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "Interaction/SaveInterface.h"
#include "Inventory/InventoryComponent.h"
#include "Item.generated.h"

struct FItemDataRow;
class UGameplayAbility;
class UPaperSprite;
class USphereComponent;

UCLASS()
class LEYR_API AItem : public APaperFlipbookActor, public IInteractionInterface, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void InitializeItemFromDataTable(const FName& RowName, bool bDespawn = false);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	virtual void Interact_Implementation(AActor* InteractingActor) override;
	virtual bool ShouldDespawn_Implementation() override { return bShouldDespawn; }
	
	//~ Save Interface
	virtual void LoadActor_Implementation() override;
	//~ Save Interface
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;
	
	UPROPERTY(EditDefaultsOnly, Category="Leyr")
	TObjectPtr<UDataTable> ItemDataTable;
	
	UPROPERTY(EditInstanceOnly, Category="Leyr")
	FDataTableRowHandle ItemRowHandle;

	UPROPERTY(BlueprintReadWrite)
	FInventoryItemData ItemData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FGameplayTag> Abilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundBase> PickupSound = nullptr;

	UFUNCTION()
	TArray<FGameplayTag> GetAbilities() { return Abilities; }

	/*
	 * Cosmetic Pick Up Effects
	*/
	UFUNCTION(BlueprintCallable)
	void StartSinusoidalMovement();
 
	UFUNCTION(BlueprintCallable)
	void StartRotation();
	
	UFUNCTION(BlueprintPure)
	bool IsSimulatingPhysics() const { return Sphere->IsSimulatingPhysics(); }
	
	UFUNCTION(BlueprintPure)
	bool ShouldBounce() const { return bShouldBounce; }
	
	UPROPERTY(BlueprintReadWrite)
	FVector CalculatedLocation;
 
	UPROPERTY(BlueprintReadWrite)
	FRotator CalculatedRotation;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leyr")
	bool bRotates = false;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leyr")
	float RotationRate = 45.f;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leyr")
	bool bSinusoidalMovement = false;
 	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leyr")
	float SineAmplitude = 1.f;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leyr")
	float SinePeriodConstant = 1.f; 
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leyr")
	FVector InitialLocation;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Leyr")
	TArray<FName> RoomsToUnveil;
	
	UPROPERTY() TObjectPtr<AActor> OverlappingActor;
	UPROPERTY(SaveGame) bool bPickedUp = false;
	
	void ItemMovement(float DeltaTime);
	float RunningTime = 0.f;
	bool bShouldDespawn = false;
	bool bShouldBounce = false;
};
