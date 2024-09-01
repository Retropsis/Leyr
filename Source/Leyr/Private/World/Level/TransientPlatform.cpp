// @ Retropsis 2024-2025.

#include "World/Level/TransientPlatform.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "Interaction/PlayerInterface.h"

ATransientPlatform::ATransientPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	
	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	OverlapBox->SetupAttachment(GetRootComponent());
	
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("FlipbookComponent");
	FlipbookComponent->SetupAttachment(GetRootComponent());
	FlipbookComponent->SetPlayRate(0.f);
	
	Platform = EPlatformType::Transient;
}

void ATransientPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (FlipbookComponent->GetPlayRate() > 0.f)
	{
		float PlayRate = FlipbookComponent->GetPlaybackPosition() / FlipbookComponent->GetFlipbookLength();
		if(PlayRate > TransientPosition)
		{
			BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		}
		if(PlayRate >= .99f)
		{
			HandleOnFinishedPlaying();
		}
	}
}

void ATransientPlatform::BeginPlay()
{
	Super::BeginPlay();
	FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &ATransientPlatform::HandleOnFinishedPlaying);
	if (HasAuthority())
	{
		OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ATransientPlatform::OnBeginOverlap);
		OverlapBox->OnComponentEndOverlap.AddDynamic(this, &ATransientPlatform::OnEndOverlap);
	}
}

void ATransientPlatform::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		FlipbookComponent->SetPlayRate(TransientFadingSpeed);
		bCanSafelyBlock = false;
	}
}

void ATransientPlatform::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		bCanSafelyBlock = true;
	}
}

void ATransientPlatform::HandleOnFinishedPlaying()
{
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(OffCycleTimer, this, &ATransientPlatform::HandleOffCycleEnd, OffCycleDuration);
	}
	FlipbookComponent->SetPlayRate(0.f);
}

void ATransientPlatform::HandleOffCycleEnd()
{
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	FlipbookComponent->SetPlaybackPosition(0.f, true);
}
