// @ Retropsis 2024-2025.

#include "World/Level/VaultDownPlatform.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"

AVaultDownPlatform::AVaultDownPlatform()
{
	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>("SpriteComponent");
	SpriteComponent->SetupAttachment(GetRootComponent());
}

void AVaultDownPlatform::SetBoxCollisionEnabled_Implementation(bool bEnabled)
{
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, bEnabled ? ECR_Block : ECR_Overlap);
}
