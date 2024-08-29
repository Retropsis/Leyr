// @ Retropsis 2024-2025.

#include "AI/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Effect/StatusEffectNiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/BaseGameplayTags.h"
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
	// GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	// GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	// GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USceneComponent>("Weapon");
	Weapon->SetupAttachment(GetRootComponent());

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>("BoxTraceStart");
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>("BoxTraceEnd");
	BoxTraceEnd->SetupAttachment(GetRootComponent());
	
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();

	//TODO: Need a more modular system
	BurnStatusEffectComponent = CreateDefaultSubobject<UStatusEffectNiagaraComponent>("BurnStatusEffectComponent");
	BurnStatusEffectComponent->SetupAttachment(GetRootComponent());
	BurnStatusEffectComponent->StatusEffectTag = GameplayTags.StatusEffect_Burn;
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
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		// return Weapon->GetSocketLocation(WeaponSocketName);
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
	return Weapon->GetComponentLocation();
}

void ABaseCharacter::GetAttackAnimationData_Implementation(FVector& InBoxTraceStart, FVector& InBoxTraceEnd)
{
	InBoxTraceStart = BoxTraceStart->GetComponentLocation();
	InBoxTraceEnd = BoxTraceEnd->GetComponentLocation();
}

FTaggedMontage ABaseCharacter::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

void ABaseCharacter::Die(const FVector& DeathImpulse)
{
	bDead = true;
	//TODO: for 3D meshes, could try also with 2D Weapon
	// Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath(DeathImpulse);
}

void ABaseCharacter::Dissolve()
{
	// if (IsValid(DissolveMaterialInstance))
	// {
	// 	UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
	// 	GetMesh()->SetMaterial(0, DynamicMatInst);
	// 	StartDissolveTimeline(DynamicMatInst);
	// }
	// if (IsValid(WeaponDissolveMaterialInstance))
	// {
	// 	UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
	// 	Weapon->SetMaterial(0, DynamicMatInst);
	// 	StartWeaponDissolveTimeline(DynamicMatInst);
	// }
}

void ABaseCharacter::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	// TODO: Activate this for 3D Meshes
	// Weapon->SetSimulatePhysics(true);
	// Weapon->SetEnableGravity(true);
	// Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	// Weapon->AddImpulse(DeathImpulse * 0.1f, NAME_None, true);
	//
	// GetMesh()->SetSimulatePhysics(true);
	// GetMesh()->SetEnableGravity(true);
	// GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	// GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	// GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);
	// Dissolve();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	// GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetSimulatePhysics(true);
	GetCapsuleComponent()->AddImpulse(DeathImpulse, NAME_None, true);
	
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	bDead = true;
	BurnStatusEffectComponent->Deactivate();
	OnDeath.Broadcast(this);
}

/*
 * Paper 2D
 */
void ABaseCharacter::ChangeDirections()
{
	SetActorRotation(UKismetMathLibrary::ComposeRotators(GetActorRotation(), FRotator(0.f, 180.f, 0.f)));
}
