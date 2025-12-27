// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "MultiPartActor.h"
#include "Cage.generated.h"

class UPaperSprite;
class UPaperSpriteComponent;
class UBoxComponent;

UCLASS()
class LEYR_API ACage : public AMultiPartActor
{
	GENERATED_BODY()

public:
	ACage();
	void ConstructChain();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> Box_Door;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UPaperFlipbookComponent> DoorFlipbook;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPaperFlipbookComponent> CageFlipbook;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPaperSpriteComponent> RootSpriteComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPaperSprite> ChainSprite;
	
	UPROPERTY(EditAnywhere, Category="Leyr", meta=(ClampMin=1))
	int32 CageChainLength = 1;
	
	UPROPERTY(EditDefaultsOnly, Category="Leyr")
	float Health = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Leyr")
	float FlashStrength = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Leyr")
	float FlashDuration = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Leyr")
	FLinearColor FlashColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, Category="Leyr")
	float ShakingStrength = 10.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Leyr")
	float ShakingDuration = 5.f;
};
