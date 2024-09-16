// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "Interaction/InteractionInterface.h"
#include "Lever.generated.h"

class UBoxComponent;

UENUM(BlueprintType)
enum class ELeverType : uint8
{
	Switch,		// Turn On Off 
	Timer,			// Stays On for a Time
	SingleUse,	// Stays On indefinitely
	Weight,		// Stays On as long as has weight on
};

UENUM(BlueprintType)
enum class ELeverState : uint8
{
	None,
	On,
	Off,
	Timer,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLeverStateChanged, ELeverState NewState);

/**
 * 
 */
UCLASS()
class LEYR_API ALever : public APaperFlipbookActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ALever();
	virtual void Interact_Implementation() override;
	void HandleLeverVisualState(ELeverState NewState);
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FOnLeverStateChanged OnLeverStateChanged;
	
protected:
	virtual void BeginPlay() override;
	void HandleOnTimerEnd();
	
	UPROPERTY(EditAnywhere, Category="Platform")
	TObjectPtr<UBoxComponent> OverlapBox;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	ELeverType LeverType = ELeverType::Switch;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	float OnTime = 3.f;

	UPROPERTY(EditAnywhere, Category="Platform|Audio")
	TObjectPtr<USoundBase> SwitchOnSound = nullptr;
	
	UPROPERTY(EditAnywhere, Category="Platform|Audio")
	TObjectPtr<USoundBase> SwitchOffSound = nullptr;
	
	UPROPERTY(EditAnywhere, Category="Platform|Audio")
	TObjectPtr<USoundBase> TimerTickSound = nullptr;
	
	UPROPERTY(EditAnywhere, Category="Platform|Audio")
	TObjectPtr<USoundBase> TimerEndSound = nullptr;

private:
	FTimerHandle OnTimer;
	ELeverState LeverState = ELeverState::Off;
};
