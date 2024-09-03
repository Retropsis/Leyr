// @ Retropsis 2024-2025.

#include "World/Level/VaultDownPlatform.h"
#include "PaperGroupedSpriteComponent.h"
#include "Components/BoxComponent.h"

AVaultDownPlatform::AVaultDownPlatform()
{		
	// TileMap = CreateDefaultSubobject<UPaperGroupedSpriteComponent>("TileMap");
	// TileMap->SetupAttachment(BoxCollision);
}

void AVaultDownPlatform::SetBoxCollisionEnabled_Implementation(bool bEnabled)
{
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, bEnabled ? ECR_Block : ECR_Overlap);
}
