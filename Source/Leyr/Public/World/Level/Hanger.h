// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hanger.generated.h"

class UPaperSprite;

UENUM(BlueprintType)
enum class EHangingType : uint8
{
	Rope,
	Ladder,
	Hook,
};

class UBoxComponent;

UCLASS()
class LEYR_API AHanger : public AActor
{
	GENERATED_BODY()
	
public:	
	AHanger();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> HangingCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Hanger")
	int32 Length = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Hanger")
	TObjectPtr<UPaperSprite> FirstTile;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Hanger")
	TObjectPtr<UPaperSprite> LastTile;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Hanger")
	TArray<TObjectPtr<UPaperSprite>> Tiles;
	
	UPROPERTY(EditAnywhere, Category="Hanger")
	EHangingType HangingType = EHangingType::Ladder;
};
