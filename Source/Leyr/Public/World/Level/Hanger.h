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
	
	UFUNCTION(CallInEditor, Category="00 - Platforming")
	virtual void InitializeHangingExtent();

protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> HangingCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="00 - Platforming")
	EBuildDirection BuildDirection = EBuildDirection::None;
	
	UPROPERTY(EditAnywhere, Category="00 - Platforming")
	EHangingType HangingType = EHangingType::Ladder;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="00 - Platforming", meta=(ClampMin=1))
	int32 Length = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="00 - Platforming")
	float IgnoreCollisionTime = .6f;

	FTimerHandle IgnoreCollisionTimer;

	UFUNCTION()
	virtual void HandleIgnoreCollisionEnd() {}
};
