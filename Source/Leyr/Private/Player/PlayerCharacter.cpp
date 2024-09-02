// @ Retropsis 2024-2025.

#include "Player/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Inventory/PlayerInventoryComponent.h"
#include "Player/PlayerCharacterController.h"
#include "Player/PlayerCharacterState.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/BaseGameplayTags.h"
#include "Interaction/PlatformInterface.h"
#include "Inventory/HotbarComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/PlayerHUD.h"

APlayerCharacter::APlayerCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_OneWayPlatform, ECR_Overlap);
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(SpringArm);
	FollowCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);

	PlayerInventory = CreateDefaultSubobject<UPlayerInventoryComponent>("PlayerInventory");
	PlayerInventory->ContainerType = EContainerType::Inventory;
	
	HotbarComponent = CreateDefaultSubobject<UHotbarComponent>("Hotbar");
	HotbarComponent->ContainerType = EContainerType::Hotbar;
	
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;

	GroundPoint = CreateDefaultSubobject<USceneComponent>("GroundPoint");
	GroundPoint->SetupAttachment(GetRootComponent());

	TraceObjectType = EOT_EnemyCapsule;
	
	CharacterClass = ECharacterClass::Warrior;
	BaseWalkSpeed = 600.f;
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TraceForPlatforms();
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

void APlayerCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = !bHitReacting ? 0.f : BaseWalkSpeed;
	if(bHitReacting)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	else
	{
		GetCharacterMovement()->StopActiveMovement();
	}
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
		TargetInventory = HotbarComponent;
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
		SourceInventory = HotbarComponent;
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
	MulticastLevelUpParticles();
}

void APlayerCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 APlayerCharacter::GetXP_Implementation() const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->GetXP();
}

int32 APlayerCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 APlayerCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 APlayerCharacter::GetSkillPointsReward_Implementation(int32 Level) const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->LevelUpInfo->LevelUpInformation[Level].SkillPointAward;
}

void APlayerCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	PlayerCharacterState->AddToLevel(InPlayerLevel);
	
	if (UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		BaseASC->UpdateAbilityStatuses(PlayerCharacterState->GetCharacterLevel());
	}
}

void APlayerCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	PlayerCharacterState->AddToAttributePoints(InAttributePoints);
}

void APlayerCharacter::AddToSkillPoints_Implementation(int32 InSkillPoints)
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	PlayerCharacterState->AddToSkillPoints(InSkillPoints);
}

int32 APlayerCharacter::GetAttributePoints_Implementation() const
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->GetAttributePoints();
}

int32 APlayerCharacter::GetSkillPoints_Implementation() const
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->GetSkillPoints();
}

void APlayerCharacter::SetCombatState_Implementation(ECombatState NewState)
{
	CombatState = NewState;
}

void APlayerCharacter::SetMovementEnabled_Implementation(bool Enabled)
{
	GetCharacterMovement()->MaxWalkSpeed = !Enabled ? 0.f : BaseWalkSpeed;
	if(Enabled)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	else
	{
		// GetCharacterMovement()->StopActiveMovement();
		GetCharacterMovement()->StopMovementImmediately();
	}
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
	OnASCRegistered.Broadcast(AbilitySystemComponent);

	if(APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(GetController()))
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerCharacterController->GetHUD()))
		{
			PlayerHUD->InitOverlay(PlayerCharacterController, PlayerCharacterState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();

	//TODO: Move this to some other (PlayerState, BeginPlay)
	AbilitySystemComponent->RegisterGameplayTagEvent(FBaseGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::HitReactTagChanged);
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

void APlayerCharacter::HandleCrouching(bool bShouldCrouch)
{
	if(bShouldCrouch && bIsCrouched) return;
	
	if(bShouldCrouch && !bIsCrouched) Crouch();
	if(!bShouldCrouch && bIsCrouched) UnCrouch();
}

void APlayerCharacter::JumpButtonPressed()
{
	bIsCrouched ? TryVaultingDown() : Jump();
}

void APlayerCharacter::TraceForPlatforms() const
{
	if(GetCharacterMovement()->IsFalling())
	{
		const FVector Start = GroundPoint->GetComponentLocation();
		const FVector End = Start + FVector::DownVector * PlatformTraceDistance;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(EOT_OneWayPlatform);
		TArray<AActor*> ActorsToIgnore;
		FHitResult Hit;
		UKismetSystemLibrary::LineTraceSingleForObjects(
			this, Start, End, ObjectTypes, false, TArray<AActor*>(),
			EDrawDebugTrace::None, Hit, true);
		
		if(Hit.bBlockingHit && GetVelocity().Z < 0.f)
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_OneWayPlatform, ECR_Block);
		}
		else GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_OneWayPlatform, ECR_Overlap);
		
		if(Hit.GetActor() && Hit.GetActor()->ActorHasTag("VaultDownPlatform"))
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_OneWayPlatform, bOverlapPlatformTimerEnded ? ECR_Block : ECR_Overlap);
		}
	}
}

void APlayerCharacter::OverlapPlatformEnd()
{
	// GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_OneWayPlatform, ECR_Block);
	bOverlapPlatformTimerEnded = true;
}

void APlayerCharacter::TryVaultingDown()
{
 	const FVector Start = GroundPoint->GetComponentLocation() + FVector(0.f, 0.f, 50.f);
	const FVector End = Start + FVector::DownVector * 60.f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EOT_OneWayPlatform);
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingleForObjects(this, Start, End, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, true);
	if(Hit.bBlockingHit && Hit.GetActor()->Implements<UPlatformInterface>() && Hit.GetActor()->ActorHasTag("VaultDownPlatform"))
	{
		// IPlatformInterface::Execute_SetBoxCollisionEnabled(Hit.GetActor(), false);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_OneWayPlatform, ECR_Overlap);
		GetWorld()->GetTimerManager().SetTimer(OverlapPlatformTimer, this, &APlayerCharacter::OverlapPlatformEnd, OverlapPlatformTime);
		bOverlapPlatformTimerEnded = false;
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
