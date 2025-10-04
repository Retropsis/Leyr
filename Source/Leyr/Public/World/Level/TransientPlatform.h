// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/Platform.h"
#include "TransientPlatform.generated.h"

class UNiagaraSystem;
class APlayerCharacter;
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
	
	//~ LevelActorInterface
	virtual void ResetState_Implementation() override;
	//~ LevelActorInterface

protected:
	virtual void BeginPlay() override;
	bool ShouldPlayerTriggerTransientTime(const APlayerCharacter* PlayerCharacter) const;
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	virtual void HandleOnFinishedPlaying();

	UFUNCTION()
	void ResetCycle();
	
	UFUNCTION()
	virtual void StartCollapsing();
	void CheckForEndOfCycle();

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(VisibleAnywhere, Category="Platform")
	TObjectPtr<UPaperFlipbookComponent> FlipbookComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Platform")
	TObjectPtr<UNiagaraSystem> RubblesEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Platform")
	FVector RubblesEffectOffset = FVector(0, 0, 0);
	
	UPROPERTY(EditAnywhere, Category="Platform")
	bool bFallingPlatform = false;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	float TransientPosition = .5f;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	float TransientFadingSpeed = 1.f;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	float OffCycleDuration = 2.f;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	int32 CycleAmount = 1;
	
	int32 CycleCount = 0;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	bool bDelayTransientTime = false;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	float PlatformTime = 1.f;

private:
	FTimerHandle PlatformTimer;
	FTimerHandle OffCycleTimer;
	bool bCanSafelyBlock = true;
	bool bCanOverlap = true;
};
