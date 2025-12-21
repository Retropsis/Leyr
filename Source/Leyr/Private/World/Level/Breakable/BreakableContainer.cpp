// @ Retropsis 2024-2025.

#include "World/Level/Breakable/BreakableContainer.h"
#include "Components/BoxComponent.h"
#include "Leyr/Leyr.h"

ABreakableContainer::ABreakableContainer()
{
	HitBox->SetCollisionObjectType(ECC_Enemy);
	HitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	// HitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	// HitBox->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	// HitBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}

void ABreakableContainer::ResetState_Implementation()
{
	
}

void ABreakableContainer::DestroyActor_Implementation()
{
	Super::DestroyActor_Implementation();
	SpawnLoot();
}
