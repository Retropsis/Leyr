// @ Retropsis 2024-2025.

#include "World/Level/Ladder.h"
#include "Components/BoxComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Leyr/Leyr.h"

ALadder::ALadder()
{
	BuildDirection = EBuildPattern::Vertical;
	HangingCollision->SetCollisionObjectType(ECC_Interaction);
	HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HangingCollision->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);

	LadderTop = CreateDefaultSubobject<UBoxComponent>("LadderTop");
	LadderTop->SetupAttachment(GetRootComponent());
	LadderTop->SetBoxExtent(FVector{ 32.f, 100.f, 8.f });
	LadderTop->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LadderTop->SetCollisionObjectType(ECC_Interaction);
	LadderTop->SetCollisionResponseToAllChannels(ECR_Ignore);
	LadderTop->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	LadderTop->SetCollisionResponseToChannel(ECC_Player, ECR_Block);
	LadderTop->SetCollisionResponseToChannel(ECC_Enemy, ECR_Block);
}

void ALadder::InitializeHangingExtent()
{
	Super::InitializeHangingExtent();
	LadderTop->SetRelativeLocation(FVector{ 0.f, 0.f, HangingCollision->GetScaledBoxExtent().Z - LadderTop->GetScaledBoxExtent().Z });
}

void ALadder::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		HangingCollision->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnBeginOverlap);
		HangingCollision->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnEndOverlap);
	}
}

void ALadder::Interact_Implementation(AActor* InteractingActor)
{
	ICombatInterface::Execute_SetCombatStateToHandle(InteractingActor, ECombatState::HangingLadder, FCombatStateParams());
}

void ALadder::InteractBottom_Implementation(AActor* InteractingActor)
{
	if (!InteractingActor->Implements<UPlayerInterface>()) return;
	
	LadderTop->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	LadderTop->SetCollisionResponseToChannel(ECC_Player, ECR_Ignore);
	const FVector Location{ HangingCollision->GetComponentLocation().X, 0.f, LadderTop->GetComponentLocation().Z };
	IPlayerInterface::Execute_HandleHangingOnLadder(InteractingActor, Location, false);
}

void ALadder::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		const FVector Location{HangingCollision->GetComponentLocation().X, 0.f, 0.f};
		IPlayerInterface::Execute_HandleHangingOnLadder(OtherActor, Location, false);
	}
}

void ALadder::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		if (const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(IgnoreCollisionTimer, this, &ALadder::HandleIgnoreCollisionEnd, IgnoreCollisionTime);
		IPlayerInterface::Execute_HandleHangingOnLadder(OtherActor, FVector::ZeroVector, true);
		
		LadderTop->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		LadderTop->SetCollisionResponseToChannel(ECC_Player, ECR_Block);
	}
}

void ALadder::HandleIgnoreCollisionEnd()
{
	HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}
