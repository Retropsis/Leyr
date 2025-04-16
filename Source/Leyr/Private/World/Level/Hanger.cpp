// @ Retropsis 2024-2025.

#include "World/Level/Hanger.h"
#include "Components/BoxComponent.h"

AHanger::AHanger()
{
	PrimaryActorTick.bCanEverTick = false;
	
	HangingCollision = CreateDefaultSubobject<UBoxComponent>("HangingCollision");
	SetRootComponent(HangingCollision);
}

void AHanger::InitializeHangingExtent()
{
	switch (BuildDirection) {
	case EBuildPattern::None:
		break;
	case EBuildPattern::Horizontal:
		HangingCollision->SetBoxExtent(FVector{ Length * 32.f, 100.f, 16.f });
		break;
	case EBuildPattern::Vertical:
		HangingCollision->SetBoxExtent(FVector{ 16.f, 100.f, Length * 32.f });
		break;
	case EBuildPattern::Diagonal_Up:
		break;
	case EBuildPattern::Diagonal_Down:
		break;
	}
}
