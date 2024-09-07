// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/Platform.h"
#include "TransientPlatform.generated.h"

class UPaperFlipbookComponent;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class LEYR_API ATransientPlatform : public APlatform
{
	GENERATED_BODY()

public:
	ATransientPlatform();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void HandleOnFinishedPlaying();

	UFUNCTION()
	void HandleOffCycleEnd() const;
	
	UFUNCTION()
	virtual void HandlePlatformTimeEnd();
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	UPROPERTY(VisibleAnywhere, Category="Platform")
	TObjectPtr<UPaperFlipbookComponent> FlipbookComponent;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	bool bFallingPlatform = false;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	float TransientPosition = .5f;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	float TransientFadingSpeed = 1.f;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	float PlatformTime = 1.f;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	float OffCycleDuration = 2.f;
	
	FTimerHandle PlatformTimer;

private:
	FTimerHandle OffCycleTimer;
	bool bCanSafelyBlock = true;
};
