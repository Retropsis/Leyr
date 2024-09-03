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
	virtual void SetBoxCollisionEnabled_Implementation(bool bEnabled) override;

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	TObjectPtr<UPaperGroupedSpriteComponent> TileMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	int32 Length = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	UPaperSprite* FirstTile = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	UPaperSprite* LastTile = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Plaform|Mechanics")
	TArray<UPaperSprite*> Tiles;
};
