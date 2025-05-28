// @ Retropsis 2024-2025.

#include "AI/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "PaperFlipbookComponent.h"
#include "PaperZDAnimInstance.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AttackSequenceInfo.h"
#include "AbilitySystem/Effect/StatusEffectNiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Data/AbilityData.h"
#include "Data/ItemData.h"
#include "Game/BaseGameplayTags.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Leyr/Leyr.h"
#include "Net/UnrealNetwork.h"

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
	
	StunStatusEffectComponent = CreateDefaultSubobject<UStatusEffectNiagaraComponent>("StunStatusEffectComponent");
	StunStatusEffectComponent->SetupAttachment(GetRootComponent());
	StunStatusEffectComponent->StatusEffectTag = GameplayTags.StatusEffect_Stun;

	GetSprite()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetSprite()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetSprite()->SetGenerateOverlapEvents(false);
	
	UpperBody = CreateDefaultSubobject<UPaperFlipbookComponent>("UpperBody");
	UpperBody->SetupAttachment(GetRootComponent());
	UpperBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UpperBody->SetCollisionResponseToAllChannels(ECR_Ignore);
	UpperBody->SetGenerateOverlapEvents(false);

	UpperBodyComponent = CreateDefaultSubobject<UPaperZDAnimationComponent>("UpperBodyComponent");

	WeaponFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>("WeaponFlipbook");
	WeaponFlipbook->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponFlipbook->SetupAttachment(UpperBody);
	WeaponFlipbook->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponFlipbook->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponFlipbook->SetGenerateOverlapEvents(false);

	WeaponComponent = CreateDefaultSubobject<UPaperZDAnimationComponent>("WeaponComponent");
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	DynamicSpriteInstance = GetSprite()->CreateDynamicMaterialInstance(0);
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 
	DOREPLIFETIME(ABaseCharacter, bIsStunned);
	DOREPLIFETIME(ABaseCharacter, bIsBurned);
	DOREPLIFETIME(ABaseCharacter, bIsElectrocuted);
}

/*
 * Ability System
 */
void ABaseCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ABaseCharacter::MakeAndApplyEffectToSelf(const FGameplayTag Tag, float Level) const
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	GetAbilitySystemComponent()->RemoveLooseGameplayTag(Tag);

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

void ABaseCharacter::HitReactFlashTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	AbilitySystemComponent->RemoveLooseGameplayTag(CallbackTag);
	PlayFlashEffect(HitReactFlashStrength, HitReactFlashPlayRate, HitReactFlashColor);
}

void ABaseCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}

void ABaseCharacter::BurnTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsBurned = NewCount > 0;
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

FVector ABaseCharacter::GetRelativeCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(WeaponSocket))
	{
		return GetSprite()->GetSocketTransform(WeaponSocketName, RTS_Actor).GetLocation();
	}
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon_UpperBody))
	{
		return UpperBody->GetSocketTransform(WeaponSocketName, RTS_Actor).GetLocation();
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
	return WeaponSocket->GetRelativeLocation();
}

UAbilityData* ABaseCharacter::LoadAndGetDefaultAbilityData_Implementation()
{
	return DefaultAbilityData.LoadSynchronous();
}

USoundBase* ABaseCharacter::ImpactSoundFromTag_Implementation(const FGameplayTag& MontageTag, ESequenceType SequenceType)
{
	return GetTaggedMontageInfoByTag(MontageTag, SequenceType).ImpactSound;
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

UPaperZDAnimInstance* ABaseCharacter::SetWeaponAnimInstance(const TSubclassOf<UPaperZDAnimInstance>& AnimInstance) const
{
	WeaponComponent->SetAnimInstanceClass(AnimInstance);
	return WeaponComponent->GetOrCreateAnimInstance();
}

void ABaseCharacter::HitStop(const float Duration, const float Amount)
{
	CustomTimeDilation = Amount;
	FTimerHandle HitStopTimer;
	GetWorld()->GetTimerManager().SetTimer(HitStopTimer, FTimerDelegate::CreateLambda([this] () { CustomTimeDilation = 1.f; }), Duration, false);
}

void ABaseCharacter::Die(const FVector& DeathImpulse, bool bExecute)
{
	DefeatState = bExecute ? EDefeatState::Executed : EDefeatState::Defeated;
	//TODO: for 3D meshes, could try also with 2D Weapon
	// Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath(DeathImpulse, DefeatState);
}

void ABaseCharacter::HandleDeathCapsuleComponent(const FVector& DeathImpulse)
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Player, ECR_Ignore);
	GetCapsuleComponent()->SetSimulatePhysics(true);
	GetCapsuleComponent()->AddImpulse(DeathImpulse, NAME_None, true);
}

void ABaseCharacter::HandleDeath(EDefeatState InDefeatState)
{
	UGameplayStatics::PlaySoundAtLocation(this, DefeatedSoundLoaded, GetActorLocation(), GetActorRotation());
	
	GetAbilitySystemComponent()->AddLooseGameplayTag(FBaseGameplayTags::Get().Defeated);
	DefeatState = InDefeatState;
	BurnStatusEffectComponent->DeactivateImmediate();
	StunStatusEffectComponent->DeactivateImmediate();
	OnDeath.Broadcast(this);
}

void ABaseCharacter::MulticastHandleDeath_Implementation(const FVector& DeathImpulse, EDefeatState InDefeatState)
{
	HandleDeathCapsuleComponent(DeathImpulse);
	// UAssetManager::GetStreamableManager().RequestAsyncLoad(DefeatedSound.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &ABaseCharacter::PlayDeathSound), FStreamableManager::DefaultAsyncLoadPriority);
	HandleDeath(InDefeatState);
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

void ABaseCharacter::AdjustDirectionToFaceTarget_Implementation(AActor* Target)
{
	if (FVector::DotProduct(GetActorForwardVector(), (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal()) < 0.f)
	{
		ChangeDirections();
	}
	// if((FMath::Sign(GetActorForwardVector().X) < 0.f && GetActorRotation().Yaw != 180.f) || (FMath::Sign(GetActorForwardVector().X) > 0.f && GetActorRotation().Yaw != 0.f))
	// {
	// 	SetActorRotation(UKismetMathLibrary::ComposeRotators(GetActorRotation(), FRotator(0.f, 180.f, 0.f)));
	// }
}

void ABaseCharacter::AddImpulse_Implementation(FVector Impulse)
{
	if(Impulse.IsNearlyZero()) return;
	GetCharacterMovement()->AddImpulse(Impulse, true);
}

/*
 * Paper 2D
 */
void ABaseCharacter::ChangeDirections()
{
	SetActorRotation(UKismetMathLibrary::ComposeRotators(GetActorRotation(), FRotator(0.f, 180.f, 0.f)));
}
