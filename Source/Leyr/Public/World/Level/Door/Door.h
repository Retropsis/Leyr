// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Entrance.h"
#include "PaperFlipbookActor.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class AEvent;
class UPaperSpriteComponent;
class UBoxComponent;

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	Still,
	Open,
	Close,
	Opening,
	Closing,
};

UENUM(BlueprintType)
enum class EDoorType : uint8
{
	None,
	Proximity,
	Lock,
	Lever,
	Event,
	Timed,
};

UCLASS(Blueprintable, BlueprintType)
class LEYR_API ADoor : public AEntrance
{
	GENERATED_BODY()
	
public:	
	ADoor();

	UFUNCTION(BlueprintNativeEvent)
	void HandleDoorMoving();
	
	UFUNCTION(BlueprintCallable)
	virtual void HandleOnFinishedPlaying();

	UFUNCTION(BlueprintCallable)
	EDoorState GetDoorState() const { return DoorState; }
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Event")
	TObjectPtr<UBoxComponent> DoorCollision;
	
	UPROPERTY(EditDefaultsOnly, Category="Event")
	bool bIsProximityDoor = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Event")
	TObjectPtr<UBoxComponent> OverlapBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Event")
	TObjectPtr<UPaperSpriteComponent> Background;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Event")
	TObjectPtr<UPaperSpriteComponent> Foreground;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Event")
	TObjectPtr<USceneComponent> OpenPosition;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Event")
	FVector CloseLocation = FVector::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Event")
	FVector OpenLocation = FVector::ZeroVector;

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void HandleDoorState(bool bOpen);

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category="Event")
	TObjectPtr<AEvent> EventOwner;

private:
	EDoorState DoorState = EDoorState::Close;
	EDoorType DoorType = EDoorType::Lock;
};
