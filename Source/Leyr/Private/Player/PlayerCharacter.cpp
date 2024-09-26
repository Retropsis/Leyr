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
	
	RopeHangingCollision = CreateDefaultSubobject<UBoxComponent>("RopeHangingCollision");
	RopeHangingCollision->SetupAttachment(GetRootComponent());
	RopeHangingCollision->ComponentTags.Add(FName("RopeCollision"));

	HalfHeightCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("HalfHeightCapsuleComponent");
	HalfHeightCapsuleComponent->SetupAttachment(GetCapsuleComponent());
	HalfHeightCapsuleComponent->SetCapsuleHalfHeight(44.f);
	HalfHeightCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TraceObjectType = EOT_EnemyCapsule;
	
	CharacterClass = ECharacterClass::Warrior;
	BaseWalkSpeed = 600.f;
	BaseWalkSpeedCrouched = 300.f;
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
	if(CombatState == ECombatState::ClimbingRope)
	{
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), MovementTarget, DeltaSeconds, MovementSpeed));
		if (FMath::IsNearlyZero(UKismetMathLibrary::Vector_Distance(GetActorLocation(), MovementTarget), 5.f))
		{
			HandleCombatState(ECombatState::Unoccupied);
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
		MakeAndApplyEffectToSelf(FBaseGameplayTags::Get().CombatState_Falling);
		// CombatState = ECombatState::Falling;
	}
	else
	{
		GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FBaseGameplayTags::Get().CombatState_Falling.GetSingleTagContainer());
	}
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Init Ability Actor Info Server Side
	InitAbilityActorInfo();
	AddCharacterAbilities();
	GetCharacterMovement()->GravityScale = BaseGravityScale;
	OnCharacterMovementUpdated.AddDynamic(this, &APlayerCharacter::HandleCharacterMovementUpdated);
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Init Ability Actor Info Client Side
	InitAbilityActorInfo();
}

void APlayerCharacter::ServerInteract_Implementation()
{
	FHitResult Hit;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	const TEnumAsByte OT = EOT_Interaction;
	ObjectTypes.Add(OT);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	UKismetSystemLibrary::BoxTraceSingleForObjects(
		this, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 25.f, FVector(50.f, 100.f, 50.f), FRotator::ZeroRotator,
		ObjectTypes, false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, Hit, false);

	if(Hit.bBlockingHit && Hit.GetActor() && Hit.GetActor()->Implements<UInteractionInterface>())
	{
		IInteractionInterface::Execute_Interact(Hit.GetActor(), this);
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

// TODO: Need a less hardcoded way to do, here we suppose first 3 indices are the 3 combos
FTaggedMontage APlayerCharacter::GetTaggedMontageByIndex_Implementation(int32 Index)
{
	return AttackSequenceInfo->OneHandedSequences.IsValidIndex(Index) ? AttackSequenceInfo->OneHandedSequences[Index] : FTaggedMontage();
}

void APlayerCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = bHitReacting ? 0.f : BaseWalkSpeedCrouched;
	if(bHitReacting)
	{
		GetCharacterMovement()->StopActiveMovement();
		HandleCombatState(ECombatState::UnCrouching);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

/*
 * Movement
 */
void APlayerCharacter::Move(const FVector2D MovementVector)
{
	// if(CombatState > ECombatState::Swimming) return;
	
	RotateController();
	
	PreviousCombatDirection = CombatDirection;
	CombatDirection = GetCombatDirectionFromVector2D(MovementVector);
	if(PreviousCombatDirection != CombatDirection) HandleCombatDirectionTag();
	
	HandleCrouching(MovementVector.Y < 0.f);
	
	switch (CombatState) {
	case ECombatState::Unoccupied:
	case ECombatState::Falling:
	case ECombatState::Crouching:
		AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		if (GetCharacterMovement()->MovementMode == MOVE_Falling) MakeAndApplyEffectToSelf(FBaseGameplayTags::Get().CombatState_Falling);
		break;
	case ECombatState::HangingRope:
		AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		if(MovementVector.Y > 0.f) HandleCombatState(ECombatState::ClimbingRope);
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
		break;
	case ECombatState::UnCrouching:
	case ECombatState::Attacking:
	case ECombatState::HangingLedge:
	case ECombatState::ClimbingRope:
	case ECombatState::OnGroundSlope:
	case ECombatState::OnRopeSlope:
	case ECombatState::HitReact:
	case ECombatState::Dodging:
	case ECombatState::Rolling:
		break;
	}
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

void APlayerCharacter::TryDodging_Implementation(){ HandleCombatState(ECombatState::Dodging); }
void APlayerCharacter::TryRolling_Implementation() { HandleCombatState(ECombatState::Rolling); }
void APlayerCharacter::ResetCombatState_Implementation(ECombatState NewState) { HandleCombatState(NewState); }

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
	if(CombatState == ECombatState::Entangled)
	{
		GetCharacterMovement()->AddImpulse(FVector::UpVector * 500.f, true);
		return;
	}
	if(CombatState == ECombatState::Swimming)
	{
		GetCharacterMovement()->AddImpulse(FVector::UpVector * 333.f, true);
		return;
	}
	if(CombatState >= ECombatState::HangingLedge)
	{
		HandleCombatState(ECombatState::Unoccupied);
		bCanGrabLedge = false;
		GetWorld()->GetTimerManager().SetTimer(OffLedgeTimer, this, &APlayerCharacter::OffLedgeEnd, OffLedgeTime);
	}
	bIsCrouched ? TryVaultingDown() : Jump();
}

void APlayerCharacter::OffLedgeEnd() { bCanGrabLedge = true; }

void APlayerCharacter::HandleCombatState(ECombatState NewState)
{
	CombatState = NewState;
	GetCharacterMovement()->GravityScale = BaseGravityScale;
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HalfHeightCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(GameplayTags.CombatStates);
	// GetAbilitySystemComponent()->RemoveActiveEffectsWithTags(CombatStates);
	// GetAbilitySystemComponent()->RemoveActiveEffectsWithAppliedTags(CombatStates);
	
	switch (CombatState) {
	case ECombatState::Unoccupied:
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		GetCharacterMovement()->MaxFlySpeed = BaseFlySpeed;
		GetCharacterMovement()->GravityScale = BaseGravityScale;
		GetCharacterMovement()->MaxAcceleration = 2048.f;
		GetCharacterMovement()->BrakingFrictionFactor = 2.f;
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		// MakeAndApplyEffectToSelf(GameplayTags.CombatState_Unoccupied);
		break;
	case ECombatState::Falling:
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		break;
	case ECombatState::Crouching:
		Crouch();
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Crouching);
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
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Ledge);
		break;
	case ECombatState::HangingRope:
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->MaxFlySpeed = RopeWalkSpeed;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Rope);
		break;
	case ECombatState::HangingLadder:
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->MaxFlySpeed = LadderWalkSpeed;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Ladder);
		break;
	case ECombatState::OnGroundSlope:
		GetCharacterMovement()->GravityScale = GroundSlopeGravityScale;
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Slope);
		break;
	case ECombatState::OnRopeSlope:
		break;
	case ECombatState::HitReact:
		break;
	case ECombatState::Entangled:
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Entangled);
		break;
	case ECombatState::Swimming:
		GetCharacterMovement()->SetMovementMode(MOVE_Swimming);
		GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = true;
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Swimming);
		break;
	case ECombatState::ClimbingRope:
		MovementSpeed = ClimbingSpeed;
		MovementTarget = GetActorLocation() + FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f - 10.f);
		break;
	case ECombatState::Dodging:
		GetCharacterMovement()->MaxWalkSpeed = DodgingSpeed;
		GetCharacterMovement()->MaxAcceleration = 5000.f;
		GetCharacterMovement()->BrakingFrictionFactor = .2f;
		GetCapsuleComponent()->SetCollisionObjectType(ECC_WorldDynamic);
		// if(const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(DodgingTimer, this, &APlayerCharacter::DodgingEnd, DodgingTime);
		break;
	case ECombatState::Rolling:
		GetCharacterMovement()->MaxWalkSpeed = RollingSpeed;
		GetCharacterMovement()->MaxAcceleration = 5000.f;
		GetCharacterMovement()->BrakingFrictionFactor = .2f;
		// HalfHeightCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		// GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// if(const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(RollingTimer, this, &APlayerCharacter::RollingEnd, RollingTime);
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
	GetCharacterMovement()->MaxWalkSpeed = !Enabled ? 0.f : BaseWalkSpeed;

	if(CombatState != ECombatState::Attacking) PreviousCombatState = CombatState;
	
	if (Enabled)
	{
		// GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		if(CombatState == ECombatState::Attacking) HandleCombatState(PreviousCombatState);
		HandleCrouching(bCrouchButtonHeld);
	}
	else
	{
		// GetCharacterMovement()->StopMovementImmediately();
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
		if(const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(EntangledExitTimer, this, &APlayerCharacter::EntangledExitEnd, EntangledExitTime);
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

void APlayerCharacter::EntangledExitEnd()
{
	HandleCombatState(ECombatState::Unoccupied);
}

void APlayerCharacter::DodgingEnd()
{
	HandleCombatState(ECombatState::Unoccupied);
}

void APlayerCharacter::RollingEnd()
{
	HandleCombatState(ECombatState::Unoccupied);
}

void APlayerCharacter::HandleSwimming_Implementation(float MinZ, float SwimmingSpeed, float SwimmingGravityScale, bool bEndOverlap)
{
	if(bEndOverlap)
	{
		if(const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(EntangledExitTimer, this, &APlayerCharacter::EntangledExitEnd, EntangledExitTime);
		GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = false;
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
	}
}

void APlayerCharacter::OverlapPlatformEnd()
{
	bOverlapPlatformTimerEnded = true;
}

void APlayerCharacter::TraceForLedge()
{
	if(!bCanGrabLedge || !GetCharacterMovement()->IsFalling()) return;;
	
	TArray<AActor*> ActorsToIgnore;
	FHitResult MidHit;
	FHitResult TopHit;
	FVector Delta = FVector(GetCapsuleComponent()->GetScaledCapsuleRadius(), 0.f, 0.f) * GetActorForwardVector().X;
	UKismetSystemLibrary::LineTraceSingle(this, GetActorLocation() , GetActorLocation()  + GetActorForwardVector() * 45.f, TraceTypeQuery1,
	false, ActorsToIgnore, EDrawDebugTrace::None, MidHit, true);
	UKismetSystemLibrary::LineTraceSingle(this, RopeHangingCollision->GetComponentLocation() , RopeHangingCollision->GetComponentLocation()  + RopeHangingCollision->GetForwardVector() * 45.f, TraceTypeQuery1,
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
		GetWorld()->GetTimerManager().SetTimer(OverlapPlatformTimer, this, &APlayerCharacter::OverlapPlatformEnd, OverlapPlatformTime);
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
	if(!GetCharacterMovement()->IsFalling() && CombatState == ECombatState::OnGroundSlope) HandleCombatState(ECombatState::Unoccupied);
	
	if(GetCharacterMovement()->IsFalling() && GetVelocity().Z < 0.f)
	{
		const FVector Start = GroundPoint->GetComponentLocation() + FVector::DownVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		const FVector End = Start + FVector::DownVector * PlatformTraceDistance * 1.5f;
		TArray<AActor*> ActorsToIgnore;
		FHitResult Hit;
		UKismetSystemLibrary::LineTraceSingle(
			this, Start, End, TraceTypeQuery1,
			false, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, Hit, true, FLinearColor::Yellow);
		
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

void APlayerCharacter::SetContainer_Implementation(AContainer* Container)
{
	InteractingContainer = Container;
	IControllerInterface::Execute_ToggleContainer(Controller, Container->GetSlotCountContainer());
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
