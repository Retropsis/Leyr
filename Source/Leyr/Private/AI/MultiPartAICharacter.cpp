// @ Retropsis 2024-2025.

#include "AI/MultiPartAICharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

AMultiPartAICharacter::AMultiPartAICharacter()
{
	MultiPartFlipbook = CreateDefaultSubobject<USkeletalMeshComponent>("MultiPartFlipbook");
	MultiPartFlipbook->SetupAttachment(GetRootComponent());

	GetSprite()->SetupAttachment(MultiPartFlipbook, HeadSocket);
	
	HealthBar->SetupAttachment(GetSprite());
}

void AMultiPartAICharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::HitReactTagChanged(CallbackTag, NewCount);
	PlayHitReactMontage();
}

void AMultiPartAICharacter::PlayMultiPartMontage(UAnimationAsset* AnimToPlay)
{
	if (MultiPartFlipbook)
	{
		MultiPartFlipbook->PlayAnimation(AnimToPlay, false);
	}
}

void AMultiPartAICharacter::MulticastHandleDeath(const FVector& DeathImpulse, EDefeatState InDefeatState)
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Player, ECR_Ignore);
	MultiPartFlipbook->SetSimulatePhysics(true);
	MultiPartFlipbook->SetEnableGravity(true);
	MultiPartFlipbook->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	MultiPartFlipbook->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MultiPartFlipbook->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	MultiPartFlipbook->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	MultiPartFlipbook->SetCollisionResponseToChannel(ECC_Player, ECR_Ignore);
	HandleDeathMultiParts();
	HandleDeath(InDefeatState);
}
