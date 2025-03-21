// @ Retropsis 2024-2025.

#include "Player/PlayerCharacterController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Player/PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Interaction/BaseUserWidgetInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/Input/BaseInputComponent.h"
#include "UI/Controller/InventoryWidgetController.h"
#include "UI/Widget/BaseUserWidget.h"
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
		BaseInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &APlayerCharacterController::InteractButtonPressed);
		BaseInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Move);
		BaseInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerCharacterController::Move);
		BaseInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacterController::Jump);
		BaseInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacterController::StopJumping);
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
		
		BaseInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld, &ThisClass::AbilityInputTagCombo);
	}
}

void APlayerCharacterController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void APlayerCharacterController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
}

void APlayerCharacterController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
}

void APlayerCharacterController::AbilityInputTagCombo(FGameplayTag InputTag)
{
	if (GetASC()) GetASC()->AbilityInputTagCombo(InputTag);
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
	if (PlayerCharacter) PlayerCharacter->Move(Value.Get<FVector2D>());
}

void APlayerCharacterController::InteractButtonPressed(const FInputActionValue& Value)
{
	if (PlayerCharacter && Value.Get<FVector2D>().Y > 0.f) PlayerCharacter->ServerInteract();
}

void APlayerCharacterController::Jump()
{
	if (PlayerCharacter) PlayerCharacter->JumpButtonPressed();
}

void APlayerCharacterController::StopJumping()
{
	if (PlayerCharacter) PlayerCharacter->StopJumping();
}

void APlayerCharacterController::InventoryButtonPressed_Implementation()
{
	ToggleInventory();
}

void APlayerCharacterController::ToggleInputMode(UWidget* InWidgetToFocus)
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
		InputModeUIOnly.SetWidgetToFocus(InWidgetToFocus->TakeWidget());
		SetInputMode(InputModeUIOnly);
		FlushPressedKeys();
		SetShowMouseCursor(true);
		bIsInventoryOpen = true;
	}
}

UInventoryWidgetController* APlayerCharacterController::GetInventoryWidgetController(const FWidgetControllerParams& WCParams)
{
	if (InventoryWidgetController == nullptr)
	{
		InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetWidgetControllerParams(WCParams);
		PlayerCharacter = Cast<APlayerCharacter>(GetCharacter());
		if(PlayerCharacter) InventoryWidgetController->InventoryComponent = PlayerCharacter->GetPlayerInventory();
		InventoryWidgetController->BindCallbacksToDependencies();
	}
	return InventoryWidgetController;
}

APlayerCharacter* APlayerCharacterController::GetPlayerCharacter()
{
	if(PlayerCharacter == nullptr) PlayerCharacter = Cast<APlayerCharacter>(GetCharacter());
	return PlayerCharacter;
}

void APlayerCharacterController::HandleButtonPressed() const
{
	if (const IBaseUserWidgetInterface* BaseUserWidgetInterface = Cast<IBaseUserWidgetInterface>(CurrentlyHoveredButton))
	{
		BaseUserWidgetInterface->Execute_HandleButtonPressed(CurrentlyHoveredButton);
	}
}

void APlayerCharacterController::HotbarButtonPressed(int32 Index)
{
}

/*
 * UI - Damage Numbers
 */
void APlayerCharacterController::ClientShowDamageNumber_Implementation(const FUIMessageData& MessageData)
{
	if (IsValid(MessageData.TargetActor) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(MessageData.TargetActor, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(MessageData.TargetActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		FVector Root = MessageData.TargetActor->GetActorLocation();
		DamageText->SetRelativeLocation(FVector{ FMath::RandRange(Root.X - 50.f, Root.X + 50.f), 50.f,  FMath::RandRange(Root.Z - 50.f, Root.Z + 50.f) });
		DamageText->SetDamageText(MessageData);
	}
}