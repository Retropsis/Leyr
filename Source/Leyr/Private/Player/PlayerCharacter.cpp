// @ Retropsis 2024-2025.

#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterAnimInstance.h"
#include "Player/PlayerCharacterController.h"
#include "Player/PlayerCharacterState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/Data/AttackSequenceInfo.h"
#include "Inventory/PlayerInventoryComponent.h"
#include "PaperZDAnimInstance.h"
#include "Game/BaseGameplayTags.h"
#include "Interaction/InteractionInterface.h"
#include "Interaction/PlatformInterface.h"
#include "Inventory/HotbarComponent.h"
#include "Inventory/Container/Container.h"
#include "UI/PlayerHUD.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "PaperFlipbookComponent.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterInfo.h"
#include "Data/InventoryCostData.h"
#include "Interaction/ElevatorInterface.h"
#include "UI/Controller/InventoryWidgetController.h"
#include "World/Map/ParallaxController.h"

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
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Interaction, ECR_Overlap);
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
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
	
	RopeHangingCollision = CreateDefaultSubobject<UBoxComponent>("RopeHangingCollision");
	RopeHangingCollision->SetupAttachment(GetRootComponent());
	RopeHangingCollision->ComponentTags.Add(FName("RopeCollision"));
	
	SwimmingCollision = CreateDefaultSubobject<UBoxComponent>("SwimmingCollision");
	SwimmingCollision->SetupAttachment(GetRootComponent());
	SwimmingCollision->ComponentTags.Add(FName("SwimmingCollision"));

	TraceObjectType = EOT_EnemyCapsule;
	
	CharacterClass = ECharacterClass::Warrior;
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TraceForPlatforms();
	TraceForLedge();
	TraceForSlope();

	ForceMove(DeltaSeconds);
}

void APlayerCharacter::ForceMove(float DeltaSeconds)
{
	FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	if(CombatState == ECombatState::ClimbingRope)
	{
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), MovementTarget, DeltaSeconds, MovementSpeed));
		if (FMath::IsNearlyZero(UKismetMathLibrary::Vector_Distance(GetActorLocation(), MovementTarget), 5.f))
		{
			HandleCombatState(ECombatState::Unoccupied);
			GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(GameplayTags.CombatDirections);
		}
	}
	if(CombatState == ECombatState::HoppingLedge)
	{
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), MovementTarget, DeltaSeconds, 8.f));
		if (FMath::IsNearlyZero(UKismetMathLibrary::Vector_Distance(GetActorLocation(), MovementTarget), 5.f))
		{
			HandleCombatState(ECombatState::Unoccupied);
			GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(GameplayTags.CombatDirections);
		}
	}
	if(CombatState == ECombatState::Dodging)
	{
		AddMovementInput(FVector(1.f, 0.f, 0.f), -GetActorForwardVector().X);
	}
	if(CombatState == ECombatState::Rolling)
	{
		AddMovementInput(FVector(1.f, 0.f, 0.f), GetActorForwardVector().X);
	}
}

void APlayerCharacter::HandleCharacterMovementUpdated(float DeltaSeconds, FVector OldLocation, FVector OldVelocity)
{	
	if(GetCharacterMovement()->MovementMode == PreviousMovementMode) return;
	
	PreviousMovementMode = GetCharacterMovement()->MovementMode;
	if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		MakeAndApplyEffectToSelf(FBaseGameplayTags::Get().CombatState_Condition_Falling);
		// CombatState = ECombatState::Falling;
	}
	else
	{
		GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FBaseGameplayTags::Get().CombatState_Condition_Falling.GetSingleTagContainer());
	}
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Init Ability Actor Info Server Side
	InitAbilityActorInfo();
	InitializeCharacterInfo();
	AddCharacterAbilities();
	GetCharacterMovement()->MaxWalkSpeed = BaseRunSpeed;
	GetCharacterMovement()->GravityScale = BaseGravityScale;
	OnCharacterMovementUpdated.AddDynamic(this, &APlayerCharacter::HandleCharacterMovementUpdated);

	InitializeParallaxController();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Init Ability Actor Info Client Side
	InitAbilityActorInfo();
}

void APlayerCharacter::InitializeParallaxController()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if(AParallaxController* ParallaxController = GetWorld()->SpawnActor<AParallaxController>(AParallaxController::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, SpawnParameters))
	{
		ParallaxController->CurrentMapName = FName("Dorn");
		ParallaxController->InitializeMapParallax(this);
		ParallaxController->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void APlayerCharacter::ServerInteract_Implementation()
{	
	if(CombatState == ECombatState::HangingLadder || GetCharacterMovement()->IsFalling()) return;

	FHitResult Hit;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	const TEnumAsByte OT = EOT_Interaction;
	ObjectTypes.Add(OT);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	UKismetSystemLibrary::BoxTraceSingleForObjects(
		this, GetActorLocation(), GetActorLocation() + FVector::UpVector * 25.f, FVector(20.f, 100.f, 20.f), FRotator::ZeroRotator,
		ObjectTypes, false, TArray<AActor*>(), EDrawDebugTrace::None, Hit, false);

	if(Hit.bBlockingHit && Hit.GetActor() && Hit.GetActor()->Implements<UInteractionInterface>())
	{
		IInteractionInterface::Execute_Interact(Hit.GetActor(), this);
		return;
	}	
	if(Hit.bBlockingHit && Hit.GetActor()->ActorHasTag("Ladder"))
	{
		HandleCombatState(ECombatState::HangingLadder);
		return;
	}
	if(Hit.bBlockingHit && Hit.GetActor()->ActorHasTag("BackEntrance"))
	{
		IInteractionInterface::Execute_Interact(Hit.GetActor(), this);
		SpringArm->bEnableCameraLag = false;
		FTimerHandle DisableCameraLagTimer;
		GetWorld()->GetTimerManager().SetTimer(DisableCameraLagTimer, FTimerDelegate::CreateLambda([this](){ SpringArm->bEnableCameraLag = true; }), 1.f, false);		
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

void APlayerCharacter::InitializeCharacterInfo()
{
	if(!HasAuthority()) return;
	//
	// const ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(this));
	// if (LeyrGameMode == nullptr || EncounterName == EEncounterName::Default) return;
	//
	const FCharacterDefaultInfo Info = CharacterInfo->GetCharacterDefaultInfo(CharacterName);
	ImpactEffect = Info.ImpactEffect;
	DefeatedSound = Info.DeathSound;
	HitReactSequence = Info.HitReactSequence;
	AttackSequenceInfo = Info.AttackSequenceInfo;
}

// TODO: Need a less hardcoded way to do, here we suppose first 3 indices are the 3 combos
FTaggedMontage APlayerCharacter::GetTaggedMontageByIndex_Implementation(int32 Index)
{
	return AttackSequenceInfo->OneHandedSequences.IsValidIndex(Index) ? AttackSequenceInfo->OneHandedSequences[Index] : FTaggedMontage();
}

void APlayerCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseRunSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = bHitReacting ? 0.f : BaseWalkSpeedCrouched;
	if(bHitReacting)
	{
		GetCharacterMovement()->StopActiveMovement();
		PreviousCombatState = CombatState;
		HandleCombatState(ECombatState::UnCrouching);
	}
	else
	{
		// GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		HandleCombatState(PreviousCombatState);
	}
}

void APlayerCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	HandleCombatState(ECombatState::Crouching);
}

void APlayerCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	GetSprite()->SetRelativeLocation(FVector::ZeroVector);
}

/*
 * Movement
 */
void APlayerCharacter::Move(const FVector2D MovementVector)
{
	if(CombatState >= ECombatState::Dodging) return;
	
	if(CombatState != ECombatState::Aiming) RotateController();
	
	PreviousCombatDirection = CombatDirection;
	CombatDirection = GetCombatDirectionFromVector2D(MovementVector);
	if(PreviousCombatDirection != CombatDirection) HandleCombatDirectionTag();
	
	HandleCrouching(MovementVector.Y < 0.f);
	
	switch (CombatState) {
	case ECombatState::Unoccupied:
	case ECombatState::Falling:
	case ECombatState::Crouching:
		AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		if (GetCharacterMovement()->MovementMode == MOVE_Falling) MakeAndApplyEffectToSelf(FBaseGameplayTags::Get().CombatState_Condition_Falling);
		break;
	case ECombatState::Walking:
	case ECombatState::WalkingPeaceful:
		AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		break;
	case ECombatState::Aiming:
		AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		Pitch(MovementVector.Y);
		break;
	case ECombatState::HangingRope:
		AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		if(MovementVector.Y > 0.f) HandleCombatState(ECombatState::ClimbingRope);
		break;
	case ECombatState::HangingHook:
		break;
	case ECombatState::HangingLadder:
		AddMovementInput(FVector(0.f, 0.f, 1.f), FMath::RoundToFloat(MovementVector.Y));
		if(MovementVector.X > 0.f) GetController()->SetControlRotation(FRotator::ZeroRotator);
		if(MovementVector.X < 0.f) GetController()->SetControlRotation(FRotator(0.f, 180.f, 0.f));
		break;
	case ECombatState::Entangled:
		AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		break;
	case ECombatState::Swimming:
		AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		AddMovementInput(FVector(0.f, 0.f, 1.f), FMath::RoundToFloat(MovementVector.Y) / 2.f);
		break;
	case ECombatState::Climbing:
		AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		AddMovementInput(FVector(0.f, 0.f, 1.f), FMath::RoundToFloat(MovementVector.Y));
		break;
	case ECombatState::OnElevator:
		AddMovementInput(FVector(.5f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		if(Elevator) IElevatorInterface::Execute_Move(Elevator, MovementVector.Y);
		break;
	case ECombatState::HangingLedge:
		TraceForHoppingLedge(MovementVector.X);
		break;
	case ECombatState::UnCrouching:
	case ECombatState::Attacking:
	case ECombatState::ClimbingRope:
	case ECombatState::HoppingLedge:
	case ECombatState::OnGroundSlope:
	case ECombatState::OnRopeSlope:
	case ECombatState::HitReact:
	case ECombatState::Dodging:
	case ECombatState::Rolling:
	case ECombatState::RollingEnd:
	case ECombatState::Defeated:
		break;
	}
}

void APlayerCharacter::Pitch(float InPitch)
{
	OverridePitch = FMath::Clamp(UpperBody->GetRelativeRotation().Pitch + InPitch, -45.f, 45.f);
	const FRotator ClampedRotation = FRotator(OverridePitch, 0.f, 0.f);
	UpperBody->SetRelativeRotation(ClampedRotation);
}

ECombatDirection APlayerCharacter::GetCombatDirectionFromVector2D(FVector2D MovementVector)
{
	if(MovementVector.Y > 0.f)
	{
		if(FMath::Abs(MovementVector.X) > 0.f) return ECombatDirection::ForwardUp;
		return ECombatDirection::Upward;
	}
	if(MovementVector.Y < 0.f)
	{
		if(FMath::Abs(MovementVector.X) > 0.f) return ECombatDirection::ForwardDown;
		return ECombatDirection::Downward;
	}
	return ECombatDirection::None;
}

void APlayerCharacter::HandleCombatDirectionTag() const
{	
	FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(GameplayTags.CombatDirections);
	switch (CombatDirection)
	{
	case ECombatDirection::None:
		break;
	case ECombatDirection::Upward:
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Directional_Upward);
		break;
	case ECombatDirection::Forward:
		// MakeAndApplyEffectToSelf(GameplayTags.CombatState_Directional_Forward);
		break;
	case ECombatDirection::Downward:
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Directional_Downward);
		break;
	case ECombatDirection::ForwardUp:
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Directional_ForwardUp);
		break;
	case ECombatDirection::ForwardDown:
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Directional_ForwardDown);
		break;
	}
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
	bCrouchButtonHeld = bShouldCrouch;
	if(CombatState >= ECombatState::Attacking) return;
	
	if(!bShouldCrouch)
	{
		HandleCombatState(ECombatState::UnCrouching);
	}
	if(GetCharacterMovement()->IsFalling())
	{
		HandleCombatState(ECombatState::UnCrouching);
		return;
	}
	if((bShouldCrouch && bIsCrouched) || CombatState >= ECombatState::HangingLedge) return;
	if(bShouldCrouch && !bIsCrouched && CombatState == ECombatState::Unoccupied) HandleCombatState(ECombatState::Crouching);
}

void APlayerCharacter::JumpButtonPressed()
{
	if(CombatState >= ECombatState::Aiming) return;
	if(CombatState == ECombatState::Entangled)
	{
		GetCharacterMovement()->AddImpulse(FVector::UpVector * 500.f, true);
		return;
	}
	if(CombatState == ECombatState::Swimming)
	{
		GetCharacterMovement()->AddImpulse(GetActorForwardVector() * 375.f + FVector::UpVector * 75.f, true);
		return;
	}
	if(CombatState >= ECombatState::HangingLedge)
	{
		HandleCombatState(ECombatState::Unoccupied);
		bCanGrabLedge = false;
		GetWorld()->GetTimerManager().SetTimer(OffLedgeTimer, FTimerDelegate::CreateLambda([this] () { bCanGrabLedge = true; }), OffLedgeTime, false);
		if(GetAttachParentActor()) DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	bIsCrouched ? TryVaultingDown() : Jump();
}

void APlayerCharacter::SetCombatStateToHandle_Implementation(ECombatState NewState)
{
	HandleCombatState(NewState);
}

void APlayerCharacter::HandleCombatState(ECombatState NewState)
{
	CombatState = NewState;
	GetCharacterMovement()->GravityScale = BaseGravityScale;
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// HalfHeightCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(GameplayTags.CombatStates);
	// GetAbilitySystemComponent()->RemoveActiveEffectsWithTags(CombatStates);
	// GetAbilitySystemComponent()->RemoveActiveEffectsWithAppliedTags(CombatStates);
	
	switch (CombatState) {
	case ECombatState::Unoccupied:
		GetCharacterMovement()->MaxWalkSpeed = BaseRunSpeed;
		GetCharacterMovement()->MaxFlySpeed = BaseFlySpeed;
		GetCharacterMovement()->GravityScale = BaseGravityScale;
		GetCharacterMovement()->MaxAcceleration = 2048.f;
		GetCharacterMovement()->BrakingFrictionFactor = 2.f;
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		// MakeAndApplyEffectToSelf(GameplayTags.CombatState_Unoccupied);
		break;
	case ECombatState::Falling:
		GetCharacterMovement()->MaxWalkSpeed = BaseRunSpeed;
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		break;
	case ECombatState::Walking:
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Condition_Walking);
		break;
	case ECombatState::WalkingPeaceful:
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Peaceful);
		break;
	case ECombatState::Crouching:
		Crouch();
		if (UPlayerCharacterAnimInstance* PlayerCharacterAnimInstance = Cast<UPlayerCharacterAnimInstance>(AnimationComponent->GetAnimInstance()))
		{
			PlayerCharacterAnimInstance->bIsCrouched = true;
		}
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Condition_Crouching);
		break;
	case ECombatState::UnCrouching:
		UnCrouch();
		CombatState = ECombatState::Unoccupied;
		// MakeAndApplyEffectToSelf(GameplayTags.CombatState_UnCrouching);
		// if(const UWorld* World = GetWorld())
		// {
		// 	World->GetTimerManager().SetTimer(UnCrouchingTimer, FTimerDelegate::CreateLambda([this] { HandleCombatState(ECombatState::Unoccupied); }), UnCrouchingTime, false);
		// }
		break;
	case ECombatState::Attacking:
		break;
	case ECombatState::HangingLedge:
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Condition_Ledge);
		break;
	case ECombatState::HangingRope:
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->MaxFlySpeed = RopeWalkSpeed;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Condition_Rope);
		break;
	case ECombatState::HangingHook:
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Condition_Hook);
		break;
	case ECombatState::HangingLadder:
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->MaxFlySpeed = LadderWalkSpeed;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Condition_Ladder);
		break;
	case ECombatState::OnElevator:
		// GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Elevator);
		break;
	case ECombatState::OnGroundSlope:
		GetCharacterMovement()->GravityScale = GroundSlopeGravityScale;
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Condition_Slope);
		break;
	case ECombatState::OnRopeSlope:
		break;
	case ECombatState::HitReact:
		break;
	case ECombatState::Entangled:
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Condition_Entangled);
		break;
	case ECombatState::Swimming:
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCharacterMovement()->MaxFlySpeed = SwimmingSpeed;
		// GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = true;
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Condition_Swimming);
		break;
	case ECombatState::ClimbingRope:
		MovementSpeed = ClimbingSpeed;
		MovementTarget = GetActorLocation() + FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f + 10.f);
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Transient_Rope);
		break;
	case ECombatState::HoppingLedge:
		MovementSpeed = ClimbingWalkSpeed;
		MovementTarget = GetActorLocation() + FVector(GetActorForwardVector().X * 50.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 15.f);
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Transient_Ledge);
		break;
	case ECombatState::Climbing:
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCharacterMovement()->MaxFlySpeed = ClimbingWalkSpeed;
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Condition_Climbing);
		break;
	case ECombatState::Dodging:
		GetCharacterMovement()->MaxWalkSpeed = DodgingSpeed;
		GetCharacterMovement()->MaxAcceleration = DodgingMaxAcceleration;
		GetCharacterMovement()->BrakingFrictionFactor = DodgingBrakeFrictionFactor;
		GetCapsuleComponent()->SetCollisionObjectType(ECC_WorldDynamic);
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Transient_Dodging);
		break;
	case ECombatState::Rolling:
		// GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		// GetCharacterMovement()->MaxFlySpeed = RollingSpeed;
		Crouch();
		GetCharacterMovement()->MaxWalkSpeedCrouched = RollingSpeed;
		GetCharacterMovement()->MaxAcceleration = RollingMaxAcceleration;
		GetCharacterMovement()->BrakingFrictionFactor = RollingBrakeFrictionFactor;
		GetSprite()->SetRelativeLocation(FVector(0.f, 0.f, 44.f));
		// HalfHeightCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		// GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Transient_Rolling);
		break;
	case ECombatState::RollingEnd:
		UnCrouch();
		GetSprite()->SetRelativeLocation(FVector::ZeroVector);
		CombatState = ECombatState::Unoccupied;
		GetCharacterMovement()->MaxWalkSpeedCrouched = BaseWalkSpeedCrouched;
		GetCharacterMovement()->MaxAcceleration = 2048.f;
		GetCharacterMovement()->BrakingFrictionFactor = 2.f;
		break;
	case ECombatState::Aiming:
		GetCharacterMovement()->MaxWalkSpeed = AimingWalkSpeed;
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Transient_Aiming);
		break;
	case ECombatState::Defeated:
		MakeAndApplyEffectToSelf(FBaseGameplayTags::Get().CombatState_Defeated);
		break;
	}
}

void APlayerCharacter::HandleHangingOnLadder_Implementation(FVector HangingTarget, bool bEndOverlap)
{
	if (!bEndOverlap && GetCharacterMovement()->IsFalling())
	{
		SetActorLocation(FVector(HangingTarget.X, 0.f, GetActorLocation().Z));
		HandleCombatState(ECombatState::HangingLadder);
	}
	if(bEndOverlap && CombatState == ECombatState::HangingLadder)
	{
		JumpButtonPressed();
	}
}

void APlayerCharacter::HandleHangingOnRope_Implementation(FVector HangingTarget, bool bEndOverlap)
{
	if (!bEndOverlap && GetCharacterMovement()->IsFalling())
	{
		SetActorLocation(FVector(GetActorLocation().X, 0.f, HangingTarget.Z - RopeHangingCollision->GetRelativeLocation().Z));
		HandleCombatState(ECombatState::HangingRope);
	}
	if(bEndOverlap && CombatState == ECombatState::HangingRope)
	{
		HandleCombatState(ECombatState::Falling);
	}
}

void APlayerCharacter::HandleHangingOnHook_Implementation(FVector HangingTarget, bool bEndOverlap)
{
	if (!bEndOverlap && GetCharacterMovement()->IsFalling())
	{
		SetActorLocation(FVector(HangingTarget.X, 0.f, HangingTarget.Z - RopeHangingCollision->GetRelativeLocation().Z));
		HandleCombatState(ECombatState::HangingHook);
	}
	if(bEndOverlap && CombatState == ECombatState::HangingHook)
	{
		HandleCombatState(ECombatState::Falling);
	}
}

void APlayerCharacter::HandleClimbing_Implementation(FVector HangingTarget, bool bEndOverlap)
{
	if (!bEndOverlap && GetCharacterMovement()->IsFalling())
	{
		HandleCombatState(ECombatState::Climbing);
	}
	if(bEndOverlap && CombatState == ECombatState::Climbing)
	{
		HandleCombatState(ECombatState::Falling);
	}
}

void APlayerCharacter::HandleHangingOnLedge(const FVector& HangingTarget)
{
	if (GetCharacterMovement()->IsFalling())
	{
		SetActorLocation(FVector(HangingTarget.X, 0.f, HangingTarget.Z));
		HandleCombatState(ECombatState::HangingLedge);
	}
}

void APlayerCharacter::SetMovementEnabled_Implementation(bool Enabled)
{
	GetCharacterMovement()->MaxWalkSpeed = !Enabled ? 0.f : BaseRunSpeed;

	if(CombatState != ECombatState::Attacking) PreviousCombatState = CombatState;
	
	if (Enabled)
	{
		if(CombatState == ECombatState::Attacking) HandleCombatState(PreviousCombatState);
		HandleCrouching(bCrouchButtonHeld);
	}
	else
	{
		CombatState = ECombatState::Attacking;
	}
}

/*
 * Entangled
 */
void APlayerCharacter::HandleEntangled_Implementation(float MinZ, float EntangledWalkSpeed, float EntangledGravityScale, bool bEndOverlap)
{
	if(bEndOverlap)
	{
		if(const UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(EntangledExitTimer, FTimerDelegate::CreateLambda([this] () { HandleCombatState(ECombatState::Unoccupied); }), EntangledExitTime, false);
		}
	}
	else
	{
		GetCharacterMovement()->StopMovementImmediately();
		CurrentMinZ = MinZ;
        GetCharacterMovement()->SetMovementMode(MOVE_Falling);
        GetCharacterMovement()->MaxFlySpeed = EntangledWalkSpeed;
        GetCharacterMovement()->GravityScale = EntangledGravityScale;
        HandleCombatState(ECombatState::Entangled);
	}
}

void APlayerCharacter::HandleSwimming_Implementation(float MinZ, float EnvironmentSwimmingSpeed, float SwimmingGravityScale, bool bEndOverlap)
{
	if(bEndOverlap)
	{
		// if(const UWorld* World = GetWorld())
		// {
		// 	World->GetTimerManager().SetTimer(EntangledExitTimer, FTimerDelegate::CreateLambda([this] () { HandleCombatState(ECombatState::Unoccupied); }), SwimmingExitTime, false);
		// }
		GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = false;
		HandleCombatState(ECombatState::Unoccupied);
		Jump();
	}
	else
	{
		GetCharacterMovement()->StopMovementImmediately();
		CurrentMinZ = MinZ;
		GetCharacterMovement()->MaxSwimSpeed = SwimmingSpeed;
		GetCharacterMovement()->GravityScale = SwimmingGravityScale;
		HandleCombatState(ECombatState::Swimming);
	}
}

void APlayerCharacter::HandleElevator_Implementation(APawn* InElevator, bool bEndOverlap)
{
	if(bEndOverlap)
	{
		Elevator = nullptr;
		HandleCombatState(ECombatState::Unoccupied);
	}
	else
	{
		Elevator = InElevator;
		HandleCombatState(ECombatState::OnElevator);
	}
}

void APlayerCharacter::HandleOverlapZone_Implementation(ECombatState NewState, bool bEndOverlap)
{
	if(bEndOverlap)
	{
		HandleCombatState(ECombatState::Unoccupied);
	}
	else
	{
		HandleCombatState(NewState);
	}
}

/*
 *
 */
void APlayerCharacter::SetSpriteRelativeLocation_Implementation(FVector NewLocation)
{
	GetSprite()->SetRelativeLocation(NewLocation);
}

void APlayerCharacter::ReduceWalkSpeed_Implementation(float AmountToReduce)
{
	GetCharacterMovement()->MaxFlySpeed = FMath::Max(0.f, GetCharacterMovement()->MaxFlySpeed - AmountToReduce);
}

void APlayerCharacter::SetWalkSpeed_Implementation(float NewSpeed)
{
	GetCharacterMovement()->MaxFlySpeed = FMath::Max(0.f, NewSpeed);
}

void APlayerCharacter::ToggleAiming_Implementation(bool bAiming)
{
	if(ForgetOverridePitchTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ForgetOverridePitchTimer);
	}
	HandleCombatState(bAiming ? ECombatState::Aiming : ECombatState::Unoccupied);
	if(!bAiming)
	{
		GetWorld()->GetTimerManager().SetTimer(ForgetOverridePitchTimer, FTimerDelegate::CreateLambda([this] ()
		{
			OverridePitch = 0.f;
			UpperBody->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		}), ForgetOverridePitchTime, false);
	}
}

/*
 * Platform Traces
 */
void APlayerCharacter::TraceForPlatforms() const
{	
	const FVector Start = GroundPoint->GetComponentLocation() + FVector::DownVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FVector End = Start + FVector::DownVector * PlatformTraceDistance;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EOT_OneWayPlatform);
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this, Start, End, ObjectTypes, false, TArray<AActor*>(),
		EDrawDebugTrace::None, Hit, true);
		
	if(Hit.bBlockingHit)
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_OneWayPlatform, ECR_Block);
	}
	else
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_OneWayPlatform, ECR_Overlap);
	}
	
	if(Hit.GetActor() && Hit.GetActor()->ActorHasTag("VaultDownPlatform"))
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_OneWayPlatform, bOverlapPlatformTimerEnded ? ECR_Block : ECR_Overlap);
		IPlatformInterface::Execute_SetBoxCollisionEnabled(Hit.GetActor(), bOverlapPlatformTimerEnded);
	}
	
	if(Hit.GetComponent() && Hit.GetComponent()->ComponentHasTag("Rope") && GetVelocity().Z < -10.f)
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_OneWayPlatform, ECR_Overlap);
	}
}

void APlayerCharacter::TraceForHoppingLedge(float MovementVectorX)
{
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;
	const FVector Start = RopeHangingCollision->GetComponentLocation() + GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FVector End = Start + RopeHangingCollision->GetForwardVector() * 45.f;
	UKismetSystemLibrary::LineTraceSingle(this, Start, End, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true);

	if(!Hit.bBlockingHit)
	{
		if(MovementVectorX > 0.f && GetActorForwardVector().X > 0.f || MovementVectorX < 0.f && GetActorForwardVector().X < 0.f) HandleCombatState(ECombatState::HoppingLedge);
	}
}

void APlayerCharacter::TraceForLedge()
{
	if(!bCanGrabLedge || !GetCharacterMovement()->IsFalling()) return;;
	
	TArray<AActor*> ActorsToIgnore;
	FHitResult MidHit;
	FHitResult TopHit;
	FVector Delta = FVector(GetCapsuleComponent()->GetScaledCapsuleRadius(), 0.f, 0.f) * GetActorForwardVector().X;
	UKismetSystemLibrary::LineTraceSingle(this, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 45.f, TraceTypeQuery1,
	false, ActorsToIgnore, EDrawDebugTrace::None, MidHit, true);
	UKismetSystemLibrary::LineTraceSingle(this, RopeHangingCollision->GetComponentLocation(), RopeHangingCollision->GetComponentLocation() + RopeHangingCollision->GetForwardVector() * 45.f, TraceTypeQuery1,
		false, ActorsToIgnore, EDrawDebugTrace::None, TopHit, true);

	if(MidHit.bBlockingHit && MidHit.GetActor() && MidHit.GetActor()->ActorHasTag("Platform")) return;

	if(MidHit.bBlockingHit && !TopHit.bBlockingHit)
	{		
		FHitResult LedgeHit;		
		for (int i = 0; i < (RopeHangingCollision->GetComponentLocation() - TopHit.TraceEnd).Length() + 45; i += 5)
		{
			FVector Start = RopeHangingCollision->GetComponentLocation() + FVector(static_cast<float>(i), 0.f, 0.f) * GetActorForwardVector().X;
			UKismetSystemLibrary::LineTraceSingle(this, Start, Start + FVector::DownVector * 82.f, TraceTypeQuery1,
				false, ActorsToIgnore, EDrawDebugTrace::None, LedgeHit, true);
			
			if(LedgeHit.bBlockingHit) break;
		}
		if(LedgeHit.bBlockingHit && LedgeHit.GetActor() && LedgeHit.GetActor()->ActorHasTag("VaultDownPlatform")) return;
		HandleHangingOnLedge(LedgeHit.bBlockingHit ? LedgeHit.Location - Delta : LedgeHit.TraceEnd - Delta);
	}
}

void APlayerCharacter::TryVaultingDown()
{
	if(GetCharacterMovement()->IsFalling() || GetVelocity().Z < 0.f) return;
	
	const FVector Start = GetActorLocation() /*+ FVector::DownVector * GetCapsuleComponent()->GetScaledCapsuleRadius()*/;
	const FVector End = Start + FVector::DownVector * (GetCapsuleComponent()->GetScaledCapsuleRadius() + 60.f);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EOT_OneWayPlatform);
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingleForObjects(this, Start, End, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true);
	
	if(Hit.bBlockingHit && Hit.GetActor()->Implements<UPlatformInterface>() && Hit.GetActor()->ActorHasTag("VaultDownPlatform"))
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_OneWayPlatform, ECR_Overlap);
		GetWorld()->GetTimerManager().SetTimer(OverlapPlatformTimer, FTimerDelegate::CreateLambda([this] () { bOverlapPlatformTimerEnded = true; }), OverlapPlatformTime, false);
		IPlatformInterface::Execute_SetBoxCollisionEnabled(Hit.GetActor(), false);
		bOverlapPlatformTimerEnded = false;
		UnCrouch();
		if (UPlayerCharacterAnimInstance* PlayerCharacterAnimInstance = Cast<UPlayerCharacterAnimInstance>(AnimationComponent->GetAnimInstance()))
		{
			PlayerCharacterAnimInstance->PlayVaultDownSequence();
		}
	}
}

void APlayerCharacter::TraceForSlope()
{
	if(CombatState == ECombatState::OnElevator || CombatState == ECombatState::WalkingPeaceful) return;
	
	if(!GetCharacterMovement()->IsFalling() && CombatState == ECombatState::OnGroundSlope) HandleCombatState(ECombatState::Unoccupied);
	
	if(GetCharacterMovement()->IsFalling() && GetVelocity().Z < 0.f)
	{
		const FVector Start = GroundPoint->GetComponentLocation() + FVector::DownVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		const FVector End = Start + FVector::DownVector * PlatformTraceDistance * 1.5f;
		TArray<AActor*> ActorsToIgnore;
		FHitResult Hit;
		UKismetSystemLibrary::LineTraceSingle(
			this, Start, End, TraceTypeQuery1,
			false, TArray<AActor*>(), EDrawDebugTrace::None, Hit, true, FLinearColor::Yellow);
		
		ECombatState PreviousState = CombatState;
		ECombatState NewState = ECombatState::Unoccupied;
		if(Hit.bBlockingHit)
		{
			const float DotProduct = FVector::DotProduct(FVector(1.f, 0.f, 0.f), Hit.Normal);
			const float Angle = FMath::RadiansToDegrees(acosf( FVector::DotProduct(FVector::UpVector, Hit.Normal)));
			
			if (Angle > GetCharacterMovement()->GetWalkableFloorAngle())
			{
				NewState = ECombatState::OnGroundSlope;
				if(DotProduct > 0.f) GetController()->SetControlRotation(FRotator::ZeroRotator);
				else if(DotProduct < 0.f) GetController()->SetControlRotation(FRotator(0.f, 180.f, 0.f));
			}
		}
		if(PreviousState != NewState) HandleCombatState(NewState);
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
	case EContainerType::Container:
		TargetInventory = InteractingContainer->Container;
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
	case EContainerType::Container:
		SourceInventory = InteractingContainer->Container;
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

void APlayerCharacter::UpdateContainerSlots_Implementation(int32 TotalSlots)
{
	IControllerInterface::Execute_UpdateContainerSlots(Controller, TotalSlots);
}

void APlayerCharacter::ResetInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex)
{
	IControllerInterface::Execute_ResetInventorySlot(Controller, ContainerType, SlotIndex);
}

void APlayerCharacter::SetContainer_Implementation(AContainer* Container)
{
	InteractingContainer = Container;
	IControllerInterface::Execute_ToggleContainer(Controller, Container->GetSlotCountContainer());
}

void APlayerCharacter::CloseContainer_Implementation()
{
	ServerCloseContainer();
}

UInventoryComponent* APlayerCharacter::GetInventoryComponentByType_Implementation(EContainerType Type)
{
	switch (Type) {
	case EContainerType::Inventory: return PlayerInventory;
	case EContainerType::Hotbar: return HotbarComponent;
	case EContainerType::Container: return InteractingContainer ? InteractingContainer->Container : nullptr;
	case EContainerType::Equipment:
		break;
	}
	return nullptr;
}

bool APlayerCharacter::UseItem_Implementation(UItemData* Asset, int32 Amount, bool bIsSelfCost, int32& OutQuantity)
{
	if(bIsSelfCost) return PlayerInventory->UseItem(Asset, Amount, OutQuantity);
	
	const UInventoryCostData* InventoryCostData = ULeyrAbilitySystemLibrary::GetInventoryCostData(this);
	bool bHasFoundCompatibleItem = false;
	
	if(APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(Controller))
	{
		if(UInventoryWidgetController* IC =PlayerCharacterController->GetInventoryWidgetController())
		{
			if(UItemData* PreferredAsset = IC->HasCompatibleItemCostInAmmunitionSlot(Asset->CostTag))
			{		
				bHasFoundCompatibleItem = PlayerInventory->UseItem(PreferredAsset, Amount, OutQuantity);
			}
		}
	}
	
	if(InventoryCostData && Asset && !bHasFoundCompatibleItem)
	{		
		FInventoryCost InventoryCost = InventoryCostData->FindCostInfoForTag(Asset->CostTag);
		for (UItemData* CompatibleItem : InventoryCost.CompatibleItems)
		{
			if(PlayerInventory->UseItem(CompatibleItem, Amount, OutQuantity))
			{
				bHasFoundCompatibleItem = true;
				break;
			}
		}
	}
	return bHasFoundCompatibleItem;
}

void APlayerCharacter::ServerCloseContainer_Implementation()
{
	InteractingContainer->ServerStopInteracting(this);
	InteractingContainer = nullptr;
	// IControllerInterface::Execute_ToggleContainer(Controller, 0);
}

/*
 *  Player Interface - Ability System
 */
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

/*
 * Combat Interface
 */
int32 APlayerCharacter::GetCharacterLevel_Implementation()
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->GetCharacterLevel();
}

void APlayerCharacter::SetCombatState_Implementation(ECombatState NewState)
{
	CombatState = NewState;
}

void APlayerCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);
	HandleCombatState(ECombatState::Defeated);
}