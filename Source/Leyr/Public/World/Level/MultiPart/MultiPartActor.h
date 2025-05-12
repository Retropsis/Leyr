// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "MultiPartActor.generated.h"

UCLASS()
class LEYR_API AMultiPartActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AMultiPartActor();
	virtual void InteractHit_Implementation(AActor* InteractingActor, FName BoneName) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractHit(FName BoneName);
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:	
	UPROPERTY(EditDefaultsOnly)
	float ImpactImpulse = 100.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBreak = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> UnbreakableBones;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> MultiPartFlipbook;

};
