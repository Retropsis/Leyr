// @ Retropsis 2024-2025.

#include "World/Level/MultiPart/Cage.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"

ACage::ACage()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CageFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>("CageFlipbook");
	CageFlipbook->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// CageFlipbook->SetGenerateOverlapEvents(false);
	CageFlipbook->SetupAttachment(MultiPartFlipbook);
	
	Box_Door = CreateDefaultSubobject<UBoxComponent>("Box_Door");
	// Box_Door->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Box_Door->SetGenerateOverlapEvents(false);
	Box_Door->SetupAttachment(MultiPartFlipbook);
	
	DoorFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>("DoorFlipbook");
	DoorFlipbook->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// DoorFlipbook->SetGenerateOverlapEvents(false);
	DoorFlipbook->SetupAttachment(Box_Door);
	DoorFlipbook->SetRelativeLocation(FVector(0.f, 50.f, 0.f));

	RootSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>("RootSpriteComponent");
	RootSpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// RootSpriteComponent->SetGenerateOverlapEvents(false);
	RootSpriteComponent->SetupAttachment(MultiPartFlipbook, MultiPartFlipbook->GetBoneName(0));
}

void ACage::BeginPlay()
{
	Super::BeginPlay();
	MultiPartFlipbook->SetPlayRate(bShouldAnimate ? FMath::FRandRange(0.9f, 1.1f) : 0.f);
	MultiPartFlipbook->SetPosition(bShouldAnimate ? FMath::FRandRange(0.f, 20.f) : 0.f);
	if (ItemPickup.IsValid())
	{
		ItemPickup->AttachToComponent(MultiPartFlipbook, FAttachmentTransformRules{ EAttachmentRule::KeepRelative, true }, MultiPartFlipbook->GetBoneName(CageChainLength));
	}
}

void ACage::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property->GetName() == TEXT("CageChainLength") && IsValid(MultiPartFlipbook))
	{
		ConstructChain();
	}
	if (PropertyChangedEvent.Property->GetName() == TEXT("ItemPickup") && ItemPickup.IsValid())
	{
		ItemPickup->AttachToComponent(MultiPartFlipbook, FAttachmentTransformRules{ EAttachmentRule::KeepRelative, true }, MultiPartFlipbook->GetBoneName(CageChainLength));
	}
	if (PropertyChangedEvent.Property->GetName() == TEXT("bFlipCageDoor"))
	{
		Box_Door->SetRelativeLocation(FVector(56.f , 0.f, -9.f));
		Box_Door->SetRelativeRotation(FRotator(0.f, bFlipCageDoor ? -180.f : 0.f, 0.f));
		DoorFlipbook->SetRelativeLocation(FVector(bFlipCageDoor ? 56.f : -56.f, 50.f, 9.f));
		DoorFlipbook->SetRelativeRotation(FRotator(0.f, bFlipCageDoor ? -180.f : 0.f, 0.f));
		SetActorRotation(FRotator(0.f, bFlipCageDoor ? -180.f : 0.f, 0.f));
	}
}

void ACage::ConstructChain()
{
	CageChainLength = FMath::Clamp(CageChainLength, 1, MultiPartFlipbook->GetNumBones());
	CageFlipbook->AttachToComponent(MultiPartFlipbook, FAttachmentTransformRules::SnapToTargetIncludingScale, MultiPartFlipbook->GetBoneName(CageChainLength));
	Box_Door->AttachToComponent(MultiPartFlipbook, FAttachmentTransformRules::KeepRelativeTransform, MultiPartFlipbook->GetBoneName(CageChainLength));
	RootSpriteComponent->AttachToComponent(MultiPartFlipbook, FAttachmentTransformRules::SnapToTargetIncludingScale, MultiPartFlipbook->GetBoneName(0));
	Box_Door->SetRelativeLocation(FVector(56.f , 0.f, -9.f));
	Box_Door->SetRelativeRotation(FRotator(0.f, bFlipCageDoor ? -180.f : 0.f, 0.f));
	DoorFlipbook->SetRelativeLocation(FVector(bFlipCageDoor ? 56.f : -56.f, 50.f, 9.f));
	DoorFlipbook->SetRelativeRotation(FRotator(0.f, bFlipCageDoor ? -180.f : 0.f, 0.f));
	SetActorRotation(FRotator(0.f, bFlipCageDoor ? -180.f : 0.f, 0.f));
	TArray<UActorComponent*> Components;
	GetComponents(UPaperSpriteComponent::StaticClass(), Components);
	// MoveTemp(Components);
	for (UActorComponent* Component : Components)
	{
		if (IsValid(Component) && Component != RootSpriteComponent) Component->DestroyComponent();
	}
	for (int i = 1; i < CageChainLength - 1; ++i)
	{
		const FName ComponentName =  FName(FString::Printf(TEXT("ChainSprite_%s"), *MultiPartFlipbook->GetBoneName(i).ToString())) ;
		if (UPaperSpriteComponent* ChainSpriteComponent = NewObject<UPaperSpriteComponent>(this, ComponentName))
		{
			AddInstanceComponent(ChainSpriteComponent);
			ChainSpriteComponent->RegisterComponent();
			ChainSpriteComponent->AttachToComponent(MultiPartFlipbook, FAttachmentTransformRules::SnapToTargetIncludingScale, MultiPartFlipbook->GetBoneName(i));
			ChainSpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			ChainSpriteComponent->SetGenerateOverlapEvents(false);
			ChainSpriteComponent->SetSprite(ChainSprite);
		}
	}
}

void ACage::InteractHit_Implementation(AActor* InteractingActor, FName BoneName)
{
	Super::InteractHit_Implementation(InteractingActor, BoneName);

	PlayFlashEffect(FlashStrength, 1 / FlashDuration, FlashColor);

	Health -= FMath::FRandRange(15.f, 24.f);
	if (Health <= 0.f)
	{
		DoorDestroyed();
	}
	else
	{
		PlayShakeEffect(ShakingStrength, 1 / ShakingDuration);
	}
}

