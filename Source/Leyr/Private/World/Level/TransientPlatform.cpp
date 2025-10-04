// @ Retropsis 2024-2025.

#include "World/Level/TransientPlatform.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"

ATransientPlatform::ATransientPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	
	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	OverlapBox->SetupAttachment(GetRootComponent());
	
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("FlipbookComponent");
	FlipbookComponent->SetupAttachment(GetRootComponent());
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetLooping(false);
	
	Platform = EPlatformType::Transient;
}

void ATransientPlatform::BeginPlay()
{
	Super::BeginPlay();
	FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &ATransientPlatform::HandleOnFinishedPlaying);
	FlipbookComponent->SetPlaybackPosition(0.f, true);
	FlipbookComponent->Stop();
	if (HasAuthority())
	{
		OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ATransientPlatform::OnBeginOverlap);
		OverlapBox->OnComponentEndOverlap.AddDynamic(this, &ATransientPlatform::OnEndOverlap);
	}
}

void ATransientPlatform::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	FlipbookComponent->SetPlaybackPosition(0.f, true);
	FlipbookComponent->Stop();
}

void ATransientPlatform::ResetState_Implementation()
{
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	FlipbookComponent->SetPlaybackPosition(0.f, true);
	FlipbookComponent->Stop();
	FlipbookComponent->SetVisibility(true);
	bCanOverlap = true;
	CycleCount = 0;
}

void ATransientPlatform::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->Implements<UPlayerInterface>() && bCanOverlap && ShouldPlayerTriggerTransientTime(Cast<APlayerCharacter>(OtherActor)))
	{
		if (bDelayTransientTime)
		{
			GetWorld()->GetTimerManager().SetTimer(PlatformTimer, this, &ATransientPlatform::StartCollapsing, PlatformTime);
		}
		else FlipbookComponent->Play();
		if (RubblesEffect) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, RubblesEffect, GetActorLocation() + RubblesEffectOffset);
		bCanOverlap = false;
	}
}

void ATransientPlatform::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(bFallingPlatform) return;
	
	if (OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		if (!bDelayTransientTime)
		{
			FlipbookComponent->Stop();
			bCanOverlap = true;
		}
		bCanSafelyBlock = true;
	}
}

void ATransientPlatform::StartCollapsing()
{
	FlipbookComponent->Play();
}

void ATransientPlatform::HandleOnFinishedPlaying()
{
	FlipbookComponent->Stop();
	if (RubblesEffect) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, RubblesEffect, GetActorLocation() + RubblesEffectOffset);
		
	if(bFallingPlatform)
	{
		FTimerHandle VisibilityTimer;
		GetWorld()->GetTimerManager().SetTimer(VisibilityTimer, FTimerDelegate::CreateLambda([this]{ FlipbookComponent->SetVisibility(false); }), 0.1f, false);
		BoxCollision->SetEnableGravity(true);
		BoxCollision->SetSimulatePhysics(true);
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
	else
	{		
		CheckForEndOfCycle();
		BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		bCanSafelyBlock = false;
	}
}

void ATransientPlatform::CheckForEndOfCycle()
{
	CycleCount++;
	if (CycleCount >= CycleAmount)
	{
		FlipbookComponent->SetVisibility(false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(OffCycleTimer, this, &ATransientPlatform::ResetCycle, OffCycleDuration);
	}
}

void ATransientPlatform::ResetCycle()
{
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	FlipbookComponent->SetPlaybackPosition(0.f, true);
	FlipbookComponent->Stop();
	bCanOverlap = true;
}

bool ATransientPlatform::ShouldPlayerTriggerTransientTime(const APlayerCharacter* PlayerCharacter) const
{
	if (PlayerCharacter == nullptr) return false;
	
	const FVector Extrema = PlayerCharacter->GetCapsuleComponent()->Bounds.GetBoxExtrema(0);
	return UKismetMathLibrary::IsPointInBox(Extrema, OverlapBox->GetComponentLocation(), OverlapBox->GetScaledBoxExtent());
}
