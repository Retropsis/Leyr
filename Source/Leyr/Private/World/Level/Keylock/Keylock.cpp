// @ Retropsis 2024-2025.

#include "World/Level/Keylock/Keylock.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Interaction/PlayerInterface.h"
#include "Leyr/Leyr.h"

AKeylock::AKeylock()
{
	OverlapBox->SetCollisionObjectType(ECC_Interaction);
	OverlapBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapBox->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	
	LockHitBox = CreateDefaultSubobject<UBoxComponent>("Keylock");
	LockHitBox->SetupAttachment(GetRootComponent());
	LockHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LockHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);

	LockFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>("Lock");
	LockFlipbook->SetupAttachment(LockHitBox);
	LockFlipbook->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LockFlipbook->SetCollisionResponseToAllChannels(ECR_Ignore);
	LockFlipbook->SetPlayRate(0.f);

	LeverType = ELeverType::SingleUse;
}

void AKeylock::LoadActor_Implementation()
{
	Super::LoadActor_Implementation();
	if (LeverState == ELeverState::On)
	{
		LockHitBox->SetRelativeLocation(LastTransform);
		LockHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		LockHitBox->SetEnableGravity(false);
		LockHitBox->SetSimulatePhysics(false);
	}
}

void AKeylock::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AKeylock::Interact_Implementation(AActor* InteractingActor)
{
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(InteractingActor))
	{
		PlayerInterface->GetOnKeyItemUsed().AddLambda([this, InteractingActor] ()
		{
			// Open
			LeverState = ELeverState::On;
			HandleLeverVisualState(LeverState);
			LockFlipbook->SetPlaybackPositionInFrames(2, true);
			OnLeverStateChanged.Broadcast(LeverState);
			LockHitBox->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			LockHitBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
			LockHitBox->SetEnableGravity(true);
			LockHitBox->SetSimulatePhysics(true);
			LockHitBox->AddImpulse(FVector{ InteractingActor->GetActorForwardVector() *  250.f  + InteractingActor->GetActorUpVector() *  250.f }, NAME_None, true);

			FTimerHandle InertTimer;
			GetWorld()->GetTimerManager().SetTimer(InertTimer, FTimerDelegate::CreateLambda([this] ()
			{
				LockHitBox->SetEnableGravity(false);
				LockHitBox->SetSimulatePhysics(false);
				LockHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				LockHitBox->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
				LastTransform = LockHitBox->GetRelativeLocation();
			}), 4.f, false);
		});
		UAssetManager::GetStreamableManager().RequestAsyncLoad(KeyData.ToSoftObjectPath(), [this, PlayerInterface, InteractingActor] () {
			const UItemData* LoadedAsset = KeyData.Get();
			if (IsValid(LoadedAsset))
			{
				PlayerInterface->Execute_TryOpenKeylock(InteractingActor, LoadedAsset);
			}
		}, FStreamableManager::AsyncLoadHighPriority);
	}
}
