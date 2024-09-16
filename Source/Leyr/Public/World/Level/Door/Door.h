// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class ALever;
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

UCLASS(Blueprintable, BlueprintType)
class LEYR_API ADoor : public APaperFlipbookActor
{
	GENERATED_BODY()
	
public:	
	ADoor();
	void HandleDoorMoving(float DeltaSeconds) const;
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Platform")
	TObjectPtr<UBoxComponent> DoorCollision;
	
	UPROPERTY(EditDefaultsOnly, Category="Platform")
	bool bIsProximityDoor = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Platform")
	TObjectPtr<UBoxComponent> OverlapBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Platform")
	TObjectPtr<UPaperSpriteComponent> Background;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Platform")
	TObjectPtr<UPaperSpriteComponent> Foreground;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Platform")
	TObjectPtr<USceneComponent> OpenPosition;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Platform")
	FVector CloseLocation = FVector::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Platform")
	FVector OpenLocation = FVector::ZeroVector;	

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UFUNCTION()
	virtual void HandleOnFinishedPlaying();
	void HandleDoorState(bool bOpen);

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category="Platform")
	TObjectPtr<ALever> Switch;

private:
	EDoorState DoorState = EDoorState::Close;
};
