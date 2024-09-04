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
	FlipbookComponent->SetLooping(false);
	
	Platform = EPlatformType::Transient;
}

void ATransientPlatform::BeginPlay()
{
	Super::BeginPlay();
	FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &ATransientPlatform::HandleOnFinishedPlaying);
	FlipbookComponent->SetPlaybackPosition(0.f, true);
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
		if (const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(PlatformTimer, this, &ATransientPlatform::HandlePlatformTimeEnd, PlatformTime);
	}
}

void ATransientPlatform::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(bFallingPlatform) return;
	
	if (OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		bCanSafelyBlock = true;
	}
}

void ATransientPlatform::HandlePlatformTimeEnd()
{
	FlipbookComponent->SetPlayRate(TransientFadingSpeed);
	FlipbookComponent->PlayFromStart();

	if(bFallingPlatform)
	{
		BoxCollision->SetEnableGravity(true);
		BoxCollision->SetSimulatePhysics(true);
		// BoxCollision->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	else
	{
		BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
        bCanSafelyBlock = false;
	}
}

void ATransientPlatform::HandleOnFinishedPlaying()
{
	if(bFallingPlatform) SetLifeSpan(.1f);
	
	else if (const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(OffCycleTimer, this, &ATransientPlatform::HandleOffCycleEnd, OffCycleDuration);
}

void ATransientPlatform::HandleOffCycleEnd() const
{
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	FlipbookComponent->SetPlaybackPosition(0.f, true);
}
