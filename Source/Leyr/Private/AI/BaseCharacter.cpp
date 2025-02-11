// @ Retropsis 2024-2025.

#include "AI/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "PaperFlipbookComponent.h"
#include "PaperZDAnimInstance.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AttackSequenceInfo.h"
#include "AbilitySystem/Effect/StatusEffectNiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/BaseGameplayTags.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Leyr/Leyr.h"

ABaseCharacter::ABaseCharacter()
{
	AnimationComponent = CreateDefaultSubobject<UPaperZDAnimationComponent>("AnimationComponent");
	GetCharacterMovement()->SetPlaneConstraintEnabled(true);
	GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);

	// TODO: In case of switching to 3D Mesh, EffectActor might be triggered form Capsule and Mesh so have to choose one to Generate Overlap Events
	// GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	WeaponSocket = CreateDefaultSubobject<USceneComponent>("WeaponSocket");
	WeaponSocket->SetupAttachment(GetRootComponent());

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>("BoxTraceStart");
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>("BoxTraceEnd");
	BoxTraceEnd->SetupAttachment(GetRootComponent());
	
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();

	//TODO: Need a more modular system
	BurnStatusEffectComponent = CreateDefaultSubobject<UStatusEffectNiagaraComponent>("BurnStatusEffectComponent");
	BurnStatusEffectComponent->SetupAttachment(GetRootComponent());
	BurnStatusEffectComponent->StatusEffectTag = GameplayTags.StatusEffect_Burn;

	UpperBody = CreateDefaultSubobject<UPaperFlipbookComponent>("UpperBody");
	UpperBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UpperBody->SetupAttachment(GetRootComponent());

	UpperBodyComponent = CreateDefaultSubobject<UPaperZDAnimationComponent>("UpperBodyComponent");

	WeaponFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>("WeaponFlipbook");
	WeaponFlipbook->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponFlipbook->SetupAttachment(GetRootComponent());

	WeaponComponent = CreateDefaultSubobject<UPaperZDAnimationComponent>("WeaponComponent");
}

/*
 * Ability System
 */
void ABaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ABaseCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ABaseCharacter::AddCharacterAbilities() const
{
	UBaseAbilitySystemComponent* BaseASC = CastChecked<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	BaseASC->AddCharacterAbilities(StartupAbilities);
	BaseASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
	BaseASC->AddCharacterCommonAbilities(CommonAbilities);
	// ULeyrAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
}

void ABaseCharacter::MakeAndApplyEffectToSelf(const FGameplayTag Tag, float Level) const
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FString EffectName = FString::Printf(TEXT("CombatState_%s"), *Tag.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(EffectName));

	Effect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	
	UTargetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	InheritedTagContainer.Added.AddTag(Tag);
	AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);
	
	Effect->StackingType = EGameplayEffectStackingType::None;
	// Effect->StackLimitCount = 1;

	// const int32 Index = Effect->Modifiers.Num();
	// Effect->Modifiers.Add(FGameplayModifierInfo());
	// FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];
	//
	// ModifierInfo.ModifierMagnitude = FScalableFloat(StatusEffectDamage);
	// ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	// ModifierInfo.Attribute = UBaseAttributeSet::GetIncomingDamageAttribute();

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, Level))
	{
		// FBaseGameplayEffectContext* BaseContext = static_cast<FBaseGameplayEffectContext*>(MutableSpec->GetContext().Get());
		// TSharedPtr<FGameplayTag> StatusEffectDamageType = MakeShareable(new FGameplayTag(DamageType));
		// BaseContext->SetDamageType(StatusEffectDamageType);

		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

/*
 * Combat Interface
 */
FVector ABaseCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	// TODO: Weapon could exist even as a sprite and have its own socket
	// check(Weapon);
	// return Weapon->GetSocketLocation(WeaponTipSocketName);
	// return GetSprite()->GetSocketLocation(WeaponTipSocketName);
	// TODO: Implements this for both weapons or just hands
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(WeaponSocket))
	{
		return GetSprite()->GetSocketLocation(WeaponSocketName);
	}
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon_UpperBody))
	{
		return UpperBody->GetSocketLocation(WeaponSocketName);
	}
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		// return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		// return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
	{
		// return GetMesh()->GetSocketLocation(TailSocketName);
	}
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftFeet))
	{
		// return GetMesh()->GetSocketLocation(LeftFootSocketName);
	}
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightFeet))
	{
		// return GetMesh()->GetSocketLocation(RightFootSocketName);
	}
	// return FVector();
	return WeaponSocket->GetComponentLocation();
}

void ABaseCharacter::GetAttackAnimationData_Implementation(FVector& InBoxTraceStart, FVector& InBoxTraceEnd)
{
	InBoxTraceStart = BoxTraceStart->GetComponentLocation();
	InBoxTraceEnd = BoxTraceEnd->GetComponentLocation();
}

FBoxTraceData ABaseCharacter::GetBoxTraceDataByTag_Implementation(const FGameplayTag MontageTag, ESequenceType SequenceType)
{
	const FTaggedMontage TaggedMontage = GetTaggedMontageInfoByTag(MontageTag, SequenceType);
	BoxTraceStart->SetRelativeLocation(TaggedMontage.BoxTraceStart);
	BoxTraceEnd->SetRelativeLocation(TaggedMontage.BoxTraceEnd);
	return FBoxTraceData{
		BoxTraceStart->GetComponentLocation(),
		BoxTraceEnd->GetComponentLocation(),
		TaggedMontage.BoxTraceExtent
	};
}

TArray<FTaggedMontage> ABaseCharacter::GetAttackMontages_Implementation(ESequenceType SequenceType)
{
	checkf(AttackSequenceInfo, TEXT("AttackSequenceInfo is missing on [%s]"), *GetNameSafe(this));
	return AttackSequenceInfo->GetSequencesByType(SequenceType);
}

FTaggedMontage ABaseCharacter::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag, ESequenceType SequenceType)
{
	return GetTaggedMontageInfoByTag(MontageTag, SequenceType);
}

FTaggedMontage ABaseCharacter::GetTaggedMontageInfoByTag(const FGameplayTag& MontageTag, ESequenceType SequenceType) const
{
	checkf(AttackSequenceInfo, TEXT("AttackSequenceInfo is missing on [%s]"), *GetNameSafe(this));

	return AttackSequenceInfo->FindSequenceInfoForTag(MontageTag, SequenceType);
}

void ABaseCharacter::Die(const FVector& DeathImpulse)
{
	bDead = true;
	//TODO: for 3D meshes, could try also with 2D Weapon
	// Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath(DeathImpulse);
}

void ABaseCharacter::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Player, ECR_Ignore);
	GetCapsuleComponent()->SetSimulatePhysics(true);
	GetCapsuleComponent()->AddImpulse(DeathImpulse, NAME_None, true);
	
	UGameplayStatics::PlaySoundAtLocation(this, DefeatedSound, GetActorLocation(), GetActorRotation());
	bDead = true;
	BurnStatusEffectComponent->Deactivate();
	OnDeath.Broadcast(this);
}

void ABaseCharacter::SetGravityScale_Implementation(float GravityValue)
{
	GetCharacterMovement()->GravityScale = GravityValue;
}

float ABaseCharacter::GetGravityScale_Implementation()
{
	return GetCharacterMovement()->GravityScale;
}

void ABaseCharacter::ResetGravityScale_Implementation()
{
	GetCharacterMovement()->GravityScale = BaseGravityScale;
}

bool ABaseCharacter::IsCharacterAirborne_Implementation()
{
	return GetCharacterMovement()->IsFalling();
}

void ABaseCharacter::SetMovementMode_Implementation(EMovementMode MovementMode,  float NewWalkingSpeed, float GravityValue)
{
	if(MovementMode == MOVE_Walking)
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		GetCharacterMovement()->GravityScale = BaseGravityScale;
		return;
	}
	
	GetCharacterMovement()->MaxWalkSpeed = NewWalkingSpeed;
	GetCharacterMovement()->GravityScale = GravityValue;
	GetCharacterMovement()->SetMovementMode(MovementMode);
}

void ABaseCharacter::AdjustDirection_Implementation()
{
	if((FMath::Sign(GetActorForwardVector().X) < 0.f && GetActorRotation().Yaw != 180.f) || (FMath::Sign(GetActorForwardVector().X) > 0.f && GetActorRotation().Yaw != 0.f))
	{
		SetActorRotation(UKismetMathLibrary::ComposeRotators(GetActorRotation(), FRotator(0.f, 180.f, 0.f)));
	}
}

void ABaseCharacter::AddImpulse_Implementation(FVector Impulse)
{
	GetCharacterMovement()->AddImpulse(Impulse, true);
}

/*
 * Paper 2D
 */
void ABaseCharacter::ChangeDirections()
{
	SetActorRotation(UKismetMathLibrary::ComposeRotators(GetActorRotation(), FRotator(0.f, 180.f, 0.f)));
}

UPaperZDAnimInstance* ABaseCharacter::SetWeaponAnimInstance(const TSubclassOf<UPaperZDAnimInstance>& AnimInstance) const
{
	WeaponComponent->SetAnimInstanceClass(AnimInstance);
	GEngine->AddOnScreenDebugMessage(554475, 5.f, FColor::Red, *WeaponComponent->GetRenderComponent()->GetName());
	return WeaponComponent->GetOrCreateAnimInstance();
	// GEngine->AddOnScreenDebugMessage(554485, 5.f, FColor::Red, *WeaponComponent->GetAnimInstance()->GetName());
	
	// WeaponComponent->InitRenderComponent(WeaponFlipbook);
}
