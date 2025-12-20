// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Event.h"
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

/**
 * 
 */
UCLASS()
class LEYR_API ALever : public AEvent
{
	GENERATED_BODY()

public:
	ALever();
	virtual void Interact_Implementation(AActor* InteractingActor) override;
	virtual bool ShouldBlockProjectile_Implementation() override { return bShouldBlockProjectile; }
	void HandleLeverVisualState(EEventState NewState);

	//~ Save Interface
	virtual void LoadActor_Implementation() override;
	//~ Save Interface
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	virtual void BeginPlay() override;
	void HandleOnTimerEnd();
	
	UPROPERTY(EditAnywhere, Category="Event")
	TObjectPtr<UBoxComponent> OverlapBox;
	
	UPROPERTY(EditAnywhere, Category="Event")
	ELeverType LeverType = ELeverType::Switch;

	UPROPERTY(SaveGame)
	FVector LastTransform;
	
	bool bShouldBlockProjectile = true;
	
	UPROPERTY(EditAnywhere, Category="Event")
	float OnTime = -1.f;

	UPROPERTY(EditAnywhere, Category="Event|Audio")
	TObjectPtr<USoundBase> SwitchOnSound = nullptr;
	
	UPROPERTY(EditAnywhere, Category="Event|Audio")
	TObjectPtr<USoundBase> SwitchOffSound = nullptr;
	
	UPROPERTY(EditAnywhere, Category="Event|Audio")
	TObjectPtr<USoundBase> TimerTickSound = nullptr;
	
	UPROPERTY(EditAnywhere, Category="Event|Audio")
	TObjectPtr<USoundBase> TimerEndSound = nullptr;

private:
	FTimerHandle OnTimer;
};
