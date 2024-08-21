// @ Retropsis 2024-2025.

#include "Player/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Inventory/PlayerInventoryComponent.h"
#include "Player/PlayerCharacterController.h"
#include "Player/PlayerCharacterState.h"
#include "UI/PlayerHUD.h"

APlayerCharacter::APlayerCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(SpringArm);
	FollowCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);

	PlayerInventory = CreateDefaultSubobject<UPlayerInventoryComponent>("PlayerInventory");
	PlayerInventory->ContainerType = EContainerType::Inventory;

	TraceObjectType = EOT_EnemyCapsule;
	
	CharacterClass = ECharacterClass::Warrior;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Init Ability Actor Info Server Side
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Init Ability Actor Info Client Side
	InitAbilityActorInfo();
}

/*
 * Player Interface
 */
void APlayerCharacter::OnSlotDrop_Implementation(EContainerType TargetContainer, EContainerType SourceContainer, int32 SourceSlotIndex, int32 TargetSlotIndex, EArmorType ArmorType)
{
	ServerOnSlotDrop(TargetContainer, SourceContainer, SourceSlotIndex, TargetSlotIndex, ArmorType);
}

void APlayerCharacter::ServerOnSlotDrop_Implementation(EContainerType TargetContainer, EContainerType SourceContainer, int32 SourceSlotIndex, int32 TargetSlotIndex, EArmorType ArmorType)
{
	UInventoryComponent* TargetInventory = nullptr;
	UInventoryComponent* SourceInventory = nullptr;
	
	switch (TargetContainer)
	{
	case EContainerType::Inventory:
		TargetInventory = PlayerInventory;
		break;
	case EContainerType::Hotbar:
		// TargetInventory = HotbarComponent;
		break;
	case EContainerType::Storage:
		break;
	case EContainerType::Equipment:
		break;
	default: ;
	}
	
	switch (SourceContainer)
	{
	case EContainerType::Inventory:
		SourceInventory = PlayerInventory;
		break;
	case EContainerType::Hotbar:
		// SourceInventory = HotbarComponent;
		break;
	case EContainerType::Storage:
		break;
	case EContainerType::Equipment:
		break;
	default: ;
	}
	if(TargetInventory && SourceInventory) TargetInventory->OnSlotDrop(SourceInventory, SourceSlotIndex, TargetSlotIndex);
}

void APlayerCharacter::UpdateInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData ItemData)
{
	IControllerInterface::Execute_UpdateInventorySlot(Controller, ContainerType, SlotIndex, ItemData);
}

void APlayerCharacter::ResetInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex)
{
	IControllerInterface::Execute_ResetInventorySlot(Controller, ContainerType, SlotIndex);
}

void APlayerCharacter::AddToXP_Implementation(int32 InXP)
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	PlayerCharacterState->AddToXP(InXP);
}

void APlayerCharacter::LevelUp_Implementation()
{
}

/*
 * Ability System
 */
void APlayerCharacter::InitAbilityActorInfo()
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	PlayerCharacterState->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerCharacterState, this);
	Cast<UBaseAbilitySystemComponent>(PlayerCharacterState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	
	AbilitySystemComponent = PlayerCharacterState->GetAbilitySystemComponent();
	AttributeSet = PlayerCharacterState->GetAttributeSet();

	if(APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(GetController()))
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerCharacterController->GetHUD()))
		{
			PlayerHUD->InitOverlay(PlayerCharacterController, PlayerCharacterState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}

void APlayerCharacter::Move(const float ScaleValue)
{
	RotateController();
	AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(ScaleValue));
}

void APlayerCharacter::RotateController() const
{
	if(GetCharacterMovement()->GetCurrentAcceleration().X > 0.f)
	{
		GetController()->SetControlRotation(FRotator::ZeroRotator);
	}
	else if(GetCharacterMovement()->GetCurrentAcceleration().X < 0.f)
	{
		GetController()->SetControlRotation(FRotator(0.f, 180.f, 0.f));
	}
}

/*
 * Combat Interface
 */
int32 APlayerCharacter::GetCharacterLevel_Implementation()
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->GetCharacterLevel();
}


