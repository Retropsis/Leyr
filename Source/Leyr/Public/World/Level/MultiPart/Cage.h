// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "MultiPartActor.h"
#include "Cage.generated.h"

UCLASS()
class LEYR_API ACage : public AMultiPartActor
{
	GENERATED_BODY()

public:
	ACage();
	virtual void InteractHit_Implementation(AActor* InteractingActor, FName BoneName) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void DoorDestroyed();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayFlashEffect(float Strength, float PlayRate, FLinearColor Color);
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayShakeEffect(float Strength, float PlayRate);
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr< UMaterialInstanceDynamic> DynamicSpriteInstance = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	FVector SpriteLocation = FVector::ZeroVector;

private:
	UPROPERTY(EditDefaultsOnly, Category="Cage")
	float Health = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Cage")
	float FlashStrength = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Cage")
	float FlashDuration = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Cage")
	FLinearColor FlashColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, Category="Cage")
	float ShakingStrength = 10.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Cage")
	float ShakingDuration = 5.f;
};
