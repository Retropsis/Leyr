// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/MultiPart/MultiPartActor.h"
#include "MPA_Light.generated.h"

class UCapsuleComponent;
/**
 * 
 */
UCLASS()
class LEYR_API AMPA_Light : public AMultiPartActor
{
	GENERATED_BODY()

public:
	AMPA_Light();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SelectFlicker();

	UFUNCTION(BlueprintCallable)
	void UpdateFlicker(float Alpha, float Scale);
	
	//~ LevelActorInterface
	virtual void ResetState_Implementation() override;
	//~ LevelActorInterface
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> LeafCapsule;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPaperFlipbookComponent> TorchComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPaperFlipbookComponent> HaloComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPointLightComponent> PointLightComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPaperFlipbook> ExtinguishFlipbook;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMaterialInstanceDynamic> MID_Halo;

private:
	UPaperFlipbook* ActiveFlipbook;
};
