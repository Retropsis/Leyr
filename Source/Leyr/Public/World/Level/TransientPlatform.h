// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/Platform.h"
#include "TransientPlatform.generated.h"

class UPaperFlipbookComponent;

/**
 * 
 */
UCLASS()
class LEYR_API ATransientPlatform : public APlatform
{
	GENERATED_BODY()

public:
	ATransientPlatform();
	void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOnFinishedPlaying();

	UFUNCTION()
	void HandleOffCycleEnd();
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Platform")
	float TransientPosition = .5f;
	
	UPROPERTY(EditDefaultsOnly, Category="Platform")
	float TransientFadingSpeed = .3f;
	
	UPROPERTY(VisibleAnywhere, Category="Platform")
	TObjectPtr<UPaperFlipbookComponent> FlipbookComponent;

	FTimerHandle OffCycleTimer;
	
	UPROPERTY(EditDefaultsOnly, Category="Platform")
	float OffCycleDuration = 2.f;

private:
	bool bCanSafelyBlock = true;
};
