// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Actor/DamageAbilityActor.h"
#include "Spikes.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class LEYR_API ASpikes : public ADamageAbilityActor
{
	GENERATED_BODY()

public:
	ASpikes();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AbilityActor|Spikes")
	TObjectPtr<UBoxComponent> BoxCollision;
};