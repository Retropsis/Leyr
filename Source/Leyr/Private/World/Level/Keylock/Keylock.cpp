// @ Retropsis 2024-2025.

#include "World/Level/Keylock/Keylock.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Leyr/Leyr.h"

AKeylock::AKeylock()
{
	LockHitBox = CreateDefaultSubobject<UBoxComponent>("Keylock");
	LockHitBox->SetupAttachment(GetRootComponent());
	LockHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LockHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	LockHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	LockHitBox->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
}

void AKeylock::LoadActor_Implementation()
{
	Super::LoadActor_Implementation();
}

void AKeylock::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		LockHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AKeylock::Interact_Implementation(AActor* InteractingActor)
{
	Super::Interact_Implementation(InteractingActor);

	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(InteractingActor))
	{
		PlayerInterface->GetOnKeyItemUsed().AddLambda([this] ()
		{
			// Open
			HandleLeverVisualState(LeverState);
			OnLeverStateChanged.Broadcast(LeverState);
		});
		PlayerInterface->Execute_TryOpenKeylock(InteractingActor, KeyData.Get());
	}
}
