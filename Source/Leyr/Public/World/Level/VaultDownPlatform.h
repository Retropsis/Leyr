// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/Platform.h"
#include "VaultDownPlatform.generated.h"

class UPaperSprite;
class UPaperGroupedSpriteComponent;

/**
 * 
 */
UCLASS()
class LEYR_API AVaultDownPlatform : public APlatform
{
	GENERATED_BODY()

public:
	AVaultDownPlatform();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void SetBoxCollisionEnabled_Implementation(bool bEnabled) override;
	virtual void BeginPlay() override;

protected:
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	UPROPERTY(EditAnywhere, Category="Plaform|Mechanics")
	TObjectPtr<UBoxComponent> OccupancyCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	int32 Length = 2;
};
