// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/Platform.h"
#include "MovingPlatform.generated.h"

class UPaperSprite;
class ALever;
class USplineComponent;

/**
 * 
 */
UCLASS()
class LEYR_API AMovingPlatform : public APlatform
{
	GENERATED_BODY()

public:
	AMovingPlatform();
	virtual void Tick(float DeltaSeconds) override;
	void Move(float DeltaSeconds);
	virtual void ChooseNextStep();
	bool HasReachedTarget() const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	UPROPERTY(EditAnywhere, Category="Platform|Route")
	float TargetTolerance = 5.f;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	float MovingSpeed = 50.f;
	
	UPROPERTY(EditAnywhere, Category="Platform")
	float InterpSpeed = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Platform|Route")
	TObjectPtr<USplineComponent> RouteSpline;

	UPROPERTY(EditAnywhere, Category="Platform")
	TObjectPtr<ALever> Switch;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	int32 Length = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	UPaperSprite* FirstTile = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	UPaperSprite* LastTile = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	UPaperSprite* LoneTile = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	TArray<UPaperSprite*> Tiles;


protected:
	virtual void BeginPlay() override;
	FVector CurrentTarget = FVector::Zero();
	FVector BoxCollisionOffset = FVector::Zero();
	int32 CurrentIndex = 0;
	bool bIsActivated = true;
};
