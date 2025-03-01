// @ Retropsis 2024-2025.

#include "World/Level/CyclingPlatform.h"
#include "Components/BoxComponent.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"

ACyclingPlatform::ACyclingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("FlipbookComponent");
	FlipbookComponent->SetupAttachment(GetRootComponent());
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	FlipbookComponent->SetLooping(false);
	
	Platform = EPlatformType::Cycling;
}

void ACyclingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (FlipbookComponent->IsPlaying())
	{
		FString Sup = FlipbookComponent->GetPlaybackPosition() / FlipbookComponent->GetFlipbookLength() > CyclingPosition ? FString("True") : FString("False");
		GEngine->AddOnScreenDebugMessage(6548, 1.f, FColor::Cyan, FString::Printf(TEXT("%f"), FlipbookComponent->GetPlaybackPosition() / FlipbookComponent->GetFlipbookLength()));
		if(FlipbookComponent->GetPlaybackPosition() / FlipbookComponent->GetFlipbookLength() > CyclingPosition)
		{
			CycleCollision(false);
		}
	}
}

void ACyclingPlatform::BeginPlay()
{
	Super::BeginPlay();
	FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &ACyclingPlatform::HandleOnFinishedPlaying);
	HandleOffCycleEnd();
}

void ACyclingPlatform::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	FlipbookComponent->SetPlaybackPosition(0.f, true);
	FlipbookComponent->Stop();
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

void ACyclingPlatform::HandleOnFinishedPlaying()
{
	if (const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(OffCycleTimer, this, &ACyclingPlatform::HandleOffCycleEnd, OffCycleDuration);
	FlipbookComponent->Stop();
}

void ACyclingPlatform::HandleOffCycleEnd()
{
	CycleCollision(true);
	FlipbookComponent->SetPlaybackPosition(0.f, true);
	if (const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(PlatformTimer, this, &ACyclingPlatform::HandlePlatformTimeEnd, PlatformTime);
}

void ACyclingPlatform::HandlePlatformTimeEnd() const
{
	FlipbookComponent->Play();
}
