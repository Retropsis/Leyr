// @ Retropsis 2024-2025.

#include "Player/PlayerCharacterController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Player/PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Player/Input/BaseInputComponent.h"
#include "UI/Widget/DamageTextComponent.h"

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	bReplicates = true;
	PlayerCharacter = Cast<APlayerCharacter>(GetCharacter());
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(DefaultMappingContext);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	
	if (UBaseInputComponent* BaseInputComponent = CastChecked<UBaseInputComponent>(InputComponent))
	{
		
		BaseInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Move);
		BaseInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacterController::Jump);
		BaseInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacterController::StopJumping);
		BaseInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacterController::InteractButtonPressed);
		BaseInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::CrouchButtonPressed);
		BaseInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacterController::CrouchButtonReleased);
		BaseInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &APlayerCharacterController::InventoryButtonPressed);

		/*
		 * Hotbar
		 */
		BaseInputComponent->BindAction(HotbarAction_0, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 9);
		BaseInputComponent->BindAction(HotbarAction_1, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 0);
		BaseInputComponent->BindAction(HotbarAction_2, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 1);
		BaseInputComponent->BindAction(HotbarAction_3, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 2);
		BaseInputComponent->BindAction(HotbarAction_4, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 3);
		BaseInputComponent->BindAction(HotbarAction_5, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 4);
		BaseInputComponent->BindAction(HotbarAction_6, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 5);
		BaseInputComponent->BindAction(HotbarAction_7, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 6);
		BaseInputComponent->BindAction(HotbarAction_8, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 7);
		BaseInputComponent->BindAction(HotbarAction_9, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 8);
		
		BaseInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
}

void APlayerCharacterController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	// GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void APlayerCharacterController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
}

void APlayerCharacterController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
}

UBaseAbilitySystemComponent* APlayerCharacterController::GetASC()
{
	if (BaseAbilitySystemComponent == nullptr)
	{
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return BaseAbilitySystemComponent;
}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void APlayerCharacterController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
}

void APlayerCharacterController::Move(const FInputActionValue& Value)
{
	if (PlayerCharacter) PlayerCharacter->Move(Value.Get<float>());
}

void APlayerCharacterController::Jump()
{
	if (PlayerCharacter) PlayerCharacter->Jump();
}

void APlayerCharacterController::StopJumping()
{
	if (PlayerCharacter) PlayerCharacter->StopJumping();
}

void APlayerCharacterController::AttackButtonPressed()
{
}

void APlayerCharacterController::CrouchButtonPressed()
{
	if (PlayerCharacter) PlayerCharacter->HandleCrouching(true);
}

void APlayerCharacterController::CrouchButtonReleased()
{
	if (PlayerCharacter) PlayerCharacter->HandleCrouching(false);
}

void APlayerCharacterController::InteractButtonPressed()
{
}

void APlayerCharacterController::InventoryButtonPressed_Implementation()
{
	// if(APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetHUD())) PlayerHUD->ToggleInventory();
	ToggleInventory();
}

void APlayerCharacterController::ToggleInputMode()
{
	if (bIsInventoryOpen)
	{
		const FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		SetShowMouseCursor(false);
		bIsInventoryOpen = false;
	}
	else
	{
		FInputModeUIOnly InputModeUIOnly;
		SetInputMode(InputModeUIOnly);
		FlushPressedKeys();
		SetShowMouseCursor(true);
		bIsInventoryOpen = true;
	}
}

void APlayerCharacterController::HotbarButtonPressed(int32 Index)
{
}

/*
 * UI - Damage Numbers
 */
void APlayerCharacterController::ClientShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}