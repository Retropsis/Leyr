// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/Platform.h"
#include "CyclingPlatform.generated.h"

class UPaperFlipbookComponent;
/**
 * 
 */
UCLASS()
class LEYR_API ACyclingPlatform : public APlatform
{
	GENERATED_BODY()

public:
	ACyclingPlatform();
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void CycleCollision(bool bShouldBlock) const;
	
	UFUNCTION()
	virtual void HandleOnFinishedPlaying();

	UFUNCTION()
	void HandleOffCycleEnd();
	
	UFUNCTION()
	void HandlePlatformTimeEnd() const;

	UPROPERTY(EditDefaultsOnly, Category="Platform")
	float CyclingPosition = .5f;
	
	UPROPERTY(EditDefaultsOnly, Category="Platform")
	float CyclingFadingSpeed = .3f;
	
	UPROPERTY(VisibleAnywhere, Category="Platform")
	TObjectPtr<UPaperFlipbookComponent> FlipbookComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Platform")
	float OffCycleDuration = 2.f;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	float PlatformTime = 2.f;

private:
	FTimerHandle OffCycleTimer;
	FTimerHandle PlatformTimer;
	bool bCanSafelyBlock = true;
};
