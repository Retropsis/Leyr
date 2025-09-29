// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "Interaction/LevelActorInterface.h"
#include "BreakableLight.generated.h"

class ULootDataSet;
class UPointLightComponent;
class UPaperFlipbook;
class UBoxComponent;
class UNiagaraSystem;

UCLASS()
class LEYR_API ABreakableLight : public APaperFlipbookActor, public IInteractionInterface, public ILevelActorInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableLight();
	virtual void InteractHit_Implementation(AActor* InteractingActor, FName BoneName) override {}
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnLoot();
	
	//~ LevelActorInterface
	virtual void ResetState_Implementation() override;
	//~ LevelActorInterface

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SelectFlicker();

	UFUNCTION(BlueprintCallable)
	void UpdateFlicker(float Alpha, float Scale);

	UFUNCTION(BlueprintCallable)
	void Extinguish();
	
	UFUNCTION(BlueprintCallable)
	void HandleOnHit(const FHitResult& HitResult);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBreak = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bHit = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPaperFlipbookComponent> HaloComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPointLightComponent> PointLightComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPaperFlipbook> ExtinguishFlipbook;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMaterialInstanceDynamic> MID_Halo;
	
	UPROPERTY(BlueprintReadOnly)
	bool bHasInteracted = false;
	
	//~ LootData
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<ULootDataSet> LootData = nullptr;

private:
	FVector StartLocation;
	UPaperFlipbook* ActiveFlipbook;
};
