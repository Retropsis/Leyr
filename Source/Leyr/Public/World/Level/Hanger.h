// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

UENUM(BlueprintType)
enum class EBuildDirection : uint8
{
	None,
	Horizontal,
	Vertical,
};

UCLASS()
class LEYR_API AHanger : public AActor
{
	GENERATED_BODY()
	
public:	
	AHanger();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void BuildTileMap();

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	TObjectPtr<UPaperGroupedSpriteComponent> TileMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> HangingCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	EBuildDirection BuildDirection = EBuildDirection::None;
	
	UPROPERTY(EditAnywhere, Category="Plaform|Mechanics")
	EHangingType HangingType = EHangingType::Ladder;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	int32 Length = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	UPaperSprite* FirstTile = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	UPaperSprite* LastTile = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	TArray<UPaperSprite*> Tiles;

	FTimerHandle IgnoreCollisionTimer;
	float IgnoreCollisionTime = 1.f;

	UFUNCTION()
	virtual void HandleIgnoreCollisionEnd() {}
};
