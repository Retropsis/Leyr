// @ Retropsis 2024-2025.

#include "World/Level/CyclingPlatform.h"
#include "Components/BoxComponent.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"

ACyclingPlatform::ACyclingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("FlipbookComponent");
	FlipbookComponent->SetupAttachment(GetRootComponent());
	
	Platform = EPlatformType::Cycling;
}

void ACyclingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (FlipbookComponent->GetPlayRate() > 0.f)
	{
		float PlayRate = FlipbookComponent->GetPlaybackPosition() / FlipbookComponent->GetFlipbookLength();
		if(PlayRate > CyclingPosition) CycleCollision(false);
		if(PlayRate >= .99f)
		{
			if (const UWorld* World = GetWorld())
			{
				World->GetTimerManager().SetTimer(OffCycleTimer, this, &ACyclingPlatform::HandleOffCycleEnd, OffCycleDuration);
			}
			FlipbookComponent->SetPlayRate(0.f);
		}
	}
}

void ACyclingPlatform::BeginPlay()
{
	Super::BeginPlay();
}

void ACyclingPlatform::CycleCollision(bool bShouldBlock) const
{
	if(bShouldBlock)
	{
		// TODO: Test this, then will have to double check if player isnt currently passing through before blocking
		BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
	else BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ACyclingPlatform::HandleOffCycleEnd()
{
	CycleCollision(true);
	FlipbookComponent->SetPlaybackPosition(0.f, true);
	FlipbookComponent->SetPlayRate(CyclingFadingSpeed);
}
