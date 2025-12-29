// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "Interaction/LevelActorInterface.h"
#include "MultiPartActor.generated.h"

class UPaperFlipbookComponent;
class UPointLightComponent;
class UPaperFlipbook;
class UNiagaraSystem;

UCLASS()
class LEYR_API AMultiPartActor : public AActor, public IInteractionInterface, public ILevelActorInterface
{
	GENERATED_BODY()
	
public:	
	AMultiPartActor();
	virtual void InteractHit_Implementation(AActor* InteractingActor, FName BoneName) override {}
	
	//~ LevelActorInterface
	virtual void ResetState_Implementation() override;
	//~ LevelActorInterface
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Extinguish();
	
	UFUNCTION(BlueprintCallable)
	void HandleOnHit(const FHitResult& HitResult);
	
	UPROPERTY(EditDefaultsOnly)
	float ImpactImpulse = 100.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBreak = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bHit = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> UnbreakableBones;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> MultiPartFlipbook;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMultiPartActor> MPAToRespawnClass;
	
private:
	FVector StartLocation;
};
