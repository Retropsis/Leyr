// @ Retropsis 2024-2025.


#include "Player/PlayerCharacterController.h"
#include "Player/PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

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
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacterController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacterController::StopJumping);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacterController::InteractButtonPressed);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacterController::CrouchButtonPressed);

		/*
		 * Hotbar
		 */
		EnhancedInputComponent->BindAction(HotbarAction_0, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 9);
		EnhancedInputComponent->BindAction(HotbarAction_1, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 0);
		EnhancedInputComponent->BindAction(HotbarAction_2, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 1);
		EnhancedInputComponent->BindAction(HotbarAction_3, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 2);
		EnhancedInputComponent->BindAction(HotbarAction_4, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 3);
		EnhancedInputComponent->BindAction(HotbarAction_5, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 4);
		EnhancedInputComponent->BindAction(HotbarAction_6, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 5);
		EnhancedInputComponent->BindAction(HotbarAction_7, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 6);
		EnhancedInputComponent->BindAction(HotbarAction_8, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 7);
		EnhancedInputComponent->BindAction(HotbarAction_9, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 8);

		// EnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void APlayerCharacterController::CursorTrace()
{
	FHitResult CursorHit;
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
}

void APlayerCharacterController::InteractButtonPressed()
{
}

void APlayerCharacterController::HotbarButtonPressed(int32 Index)
{
}
