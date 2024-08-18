// @ Retropsis 2024-2025.

#include "AI/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
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
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

/*
 * Combat Interface
 */
FVector ABaseCharacter::GetCombatSocketLocation()
{
	// TODO: Weapon could exist even as a sprite and have its own socket
	// check(Weapon);
	// return Weapon->GetSocketLocation(WeaponTipSocketName);
	// return GetSprite()->GetSocketLocation(WeaponTipSocketName);
	return Weapon->GetComponentLocation();
}

void ABaseCharacter::GetAttackAnimationData_Implementation(FVector& InBoxTraceStart, FVector& InBoxTraceEnd)
{
	InBoxTraceStart = BoxTraceStart->GetComponentLocation();
	InBoxTraceEnd = BoxTraceEnd->GetComponentLocation();
}

void ABaseCharacter::Die()
{
	bDead = true;
	// TODO: for 3D meshes, could try also with 2D Weapon
	// Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
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

void ABaseCharacter::MulticastHandleDeath_Implementation()
{
	// TODO: Activate this for 3D Meshes
	// Weapon->SetSimulatePhysics(true);
	// Weapon->SetEnableGravity(true);
	// Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	//
	// GetMesh()->SetSimulatePhysics(true);
	// GetMesh()->SetEnableGravity(true);
	// GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	// GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	// Dissolve();
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

/*
 * Paper 2D
 */
void ABaseCharacter::ChangeDirections()
{
	SetActorRotation(UKismetMathLibrary::ComposeRotators(GetActorRotation(), FRotator(0.f, 180.f, 0.f)));
}
