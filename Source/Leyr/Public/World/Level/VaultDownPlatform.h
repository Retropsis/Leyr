// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/Platform.h"
#include "VaultDownPlatform.generated.h"

class UPaperSpriteComponent;
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
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperSpriteComponent> SpriteComponent;
};
