// @ Retropsis 2024-2025.

#include "World/Level/Rope.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Leyr/Leyr.h"

ARope::ARope()
{
	BuildDirection = EBuildDirection::Horizontal;

	HangingCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	OccupancyCollision = CreateDefaultSubobject<UBoxComponent>("OccupancyCollision");
	OccupancyCollision->SetupAttachment(GetRootComponent());
	OccupancyCollision->SetRelativeLocation(FVector(0.f, 0.f, 32.f));
	OccupancyCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OccupancyCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	OccupancyCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OccupancyCollision->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
}

void ARope::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		HangingCollision->OnComponentBeginOverlap.AddDynamic(this, &ARope::OnBeginOverlap);
		HangingCollision->OnComponentEndOverlap.AddDynamic(this, &ARope::OnEndOverlap);
		OccupancyCollision->OnComponentEndOverlap.AddDynamic(this, &ARope::OnOccupancyEndOverlap);
	}
}

void ARope::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	HangingCollision->SetBoxExtent(FVector(32.f * Length, 100.f, 16.f));
	OccupancyCollision->SetBoxExtent(FVector(32.f * Length, 100.f, 16.f));
}

void ARope::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherComp && OtherComp->ComponentHasTag("RopeCollision"))
	{
		const FVector Location{0.f, 0.f, HangingCollision->GetComponentLocation().Z};
		IPlayerInterface::Execute_HandleHangingOnRope(OtherActor, Location, false);
	}
}

void ARope::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherComp && OtherComp->ComponentHasTag("RopeCollision"))
	{
		// HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		// if (const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(IgnoreCollisionTimer, this, &ARope::HandleIgnoreCollisionEnd, IgnoreCollisionTime);
		
		IPlayerInterface::Execute_HandleHangingOnRope(OtherActor, FVector::ZeroVector, true);
	}
}

void ARope::HandleIgnoreCollisionEnd()
{
	HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ARope::SetBoxCollisionEnabled_Implementation(bool bEnabled)
{
	// TODO: Probably change to ECC_PlayerCapsule
	HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, bEnabled ? ECR_Block : ECR_Overlap);
}

void ARope::OnOccupancyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor->ActorHasTag("Player")) HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}
