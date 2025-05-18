// @ Retropsis 2024-2025.

#include "AI/MultiPartAICharacter.h"
#include "PaperFlipbookComponent.h"
#include "AI/MultiPartAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

AMultiPartAICharacter::AMultiPartAICharacter()
{
	MultiPartFlipbook = CreateDefaultSubobject<USkeletalMeshComponent>("MultiPartFlipbook");
	MultiPartFlipbook->SetupAttachment(GetRootComponent());

	GetSprite()->SetupAttachment(MultiPartFlipbook, SpriteSocket);
	
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

void AMultiPartAICharacter::ChangeDirection_Implementation(EDirection NewDirection)
{
	if (MultiPartFlipbook == nullptr) return;
	if (UMultiPartAnimInstance* AnimInstance = Cast<UMultiPartAnimInstance>(MultiPartFlipbook->GetAnimInstance()))
	{
		AnimInstance->Direction = NewDirection;
	}
}

void AMultiPartAICharacter::ChangeMultiPartAnimInstance_Implementation(const TSubclassOf<UAnimInstance> NewInstance)
{
	SetMultiPartAnimInstance(NewInstance);
}

void AMultiPartAICharacter::SetMultiPartAnimInstance(const TSubclassOf<UAnimInstance> NewInstance)
{
	if (MultiPartFlipbook && NewInstance)
	{
		MultiPartFlipbook->SetAnimInstanceClass(NewInstance);
	}
}

void AMultiPartAICharacter::MulticastHandleDeath(const FVector& DeathImpulse, EDefeatState InDefeatState)
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Player, ECR_Ignore);
	// MultiPartFlipbook->SetSimulatePhysics(true);
	// MultiPartFlipbook->SetEnableGravity(true);
	// MultiPartFlipbook->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// MultiPartFlipbook->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	// MultiPartFlipbook->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	// MultiPartFlipbook->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	// MultiPartFlipbook->SetCollisionResponseToChannel(ECC_Player, ECR_Ignore);
	HandleDeathMultiParts();
	HandleDeath(InDefeatState);
}
