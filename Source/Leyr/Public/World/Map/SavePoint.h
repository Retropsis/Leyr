// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "SavePoint.generated.h"

class UBoxComponent;
class UPaperFlipbookComponent;
/**
 * 
 */
UCLASS()
class LEYR_API ASavePoint : public APlayerStart, public ISaveInterface
{
	GENERATED_BODY()

public:
	ASavePoint(const FObjectInitializer& ObjectInitializer);

	/* SaveInterface */
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	virtual void LoadActor_Implementation() override;
	/* SaveInterface */

	UPROPERTY(SaveGame)
	bool bActivated = false;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPaperFlipbookComponent> FlipbookComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> OverlapBox;
};
