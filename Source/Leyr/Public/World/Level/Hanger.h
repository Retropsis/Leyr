// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "LevelDesignData.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "Hanger.generated.h"

class UPaperSprite;
class UBoxComponent;
class UPaperGroupedSpriteComponent;

UENUM(BlueprintType)
enum class EHangingType : uint8
{
	Rope,
	Ladder,
	Hook,
};

UCLASS()
class LEYR_API AHanger : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AHanger();

protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform")
	TObjectPtr<UPaperGroupedSpriteComponent> TileMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> HangingCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform")
	EBuildDirection BuildDirection = EBuildDirection::None;
	
	UPROPERTY(EditAnywhere, Category="Plaform")
	EHangingType HangingType = EHangingType::Ladder;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform")
	int32 Length = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	float IgnoreCollisionTime = .6f;

	FTimerHandle IgnoreCollisionTimer;

	UFUNCTION()
	virtual void HandleIgnoreCollisionEnd() {}
};
