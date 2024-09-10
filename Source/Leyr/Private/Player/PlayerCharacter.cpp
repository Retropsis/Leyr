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
#include "PaperZDAnimInstance.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/BaseGameplayTags.h"
#include "Interaction/PlatformInterface.h"
#include "Inventory/HotbarComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/PlayerCharacterAnimInstance.h"
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
	
	RopeHangingCollision = CreateDefaultSubobject<UBoxComponent>("RopeHangingCollision");
	RopeHangingCollision->SetupAttachment(GetRootComponent());
	RopeHangingCollision->ComponentTags.Add(FName("RopeCollision"));

	TraceObjectType = EOT_EnemyCapsule;
	
	CharacterClass = ECharacterClass::Warrior;
	BaseWalkSpeed = 600.f;
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TraceForPlatforms();
	TraceForLedge();
	// TraceForSlope();
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Init Ability Actor Info Server Side
	InitAbilityActorInfo();
	AddCharacterAbilities();
	GetCharacterMovement()->GravityScale = BaseGravityScale;
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

void APlayerCharacter::Move(const FVector2D MovementVector)
{
	RotateController();

	// GEngine->AddOnScreenDebugMessage(1365, 1.f, FColor::Green, FString::Printf(TEXT("%s"), *MovementVector.ToString()));
	
	switch (CombatState) {
	case ECombatState::Unoccupied:
	case ECombatState::Falling:
		AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		break;
	case ECombatState::Attacking:
		// AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		break;
	case ECombatState::HangingLedge:
		break;
	case ECombatState::HangingRope:
		AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		break;
	case ECombatState::HangingLadder:
		AddMovementInput(FVector(0.f, 0.f, 1.f), FMath::RoundToFloat(MovementVector.Y));
		break;
	case ECombatState::OnGroundSlope:
		break;
	case ECombatState::OnRopeSlope:
		break;
	}
}

void APlayerCharacter::SetMovementEnabled_Implementation(bool Enabled)
{
	GetCharacterMovement()->MaxWalkSpeed = !Enabled ? 0.f : BaseWalkSpeed;

	if(CombatState != ECombatState::Attacking) PreviousCombatState = CombatState;
	
	if (Enabled)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		if(CombatState == ECombatState::Attacking) HandleCombatState(PreviousCombatState);
	}
	else
	{
		GetCharacterMovement()->StopMovementImmediately();
		CombatState = ECombatState::Attacking;
	}
}

void APlayerCharacter::HandleCombatState(ECombatState NewState)
{
	CombatState = NewState;
	
	switch (CombatState) {
	case ECombatState::Unoccupied:
		// GetCharacterMovement()->GravityScale = GravityScale;
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		break;
	case ECombatState::Attacking:
		break;
	case ECombatState::HangingLedge:
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		break;
	case ECombatState::HangingRope:
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->MaxFlySpeed = RopeWalkSpeed;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FBaseGameplayTags::Get().CombatState_Rope.GetSingleTagContainer());
		break;
	case ECombatState::HangingLadder:
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->MaxFlySpeed = LadderWalkSpeed;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		break;
	case ECombatState::Falling:
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		break;
	case ECombatState::OnGroundSlope:
		break;
	case ECombatState::OnRopeSlope:
		break;
	}
}

// TODO: Need a less hardcoded way to do, here we suppose first 3 indices are the 3 combos
FTaggedMontage APlayerCharacter::GetTaggedMontageByIndex_Implementation(int32 Index)
{
	return AttackMontages.IsValidIndex(Index) ? AttackMontages[Index] : FTaggedMontage();
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
		// JumpButtonPressed();
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

void APlayerCharacter::OffLedgeEnd() { bCanGrabLedge = true; }

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
	if(!bShouldCrouch)
	{
		UnCrouch();
		// GEngine->AddOnScreenDebugMessage(9874, 1.5, FColor::Magenta, FString::Printf(TEXT("UnCrouch From Release")));
	}
	if(GetCharacterMovement()->IsFalling())
	{
		UnCrouch();
		// GEngine->AddOnScreenDebugMessage(9875, 1.5f, FColor::Magenta, FString::Printf(TEXT("UnCrouch From Falling")));
		return;
	}
	if((bShouldCrouch && bIsCrouched) || CombatState >= ECombatState::HangingLedge) return;
	if(bShouldCrouch && !bIsCrouched) Crouch();
	// GEngine->AddOnScreenDebugMessage(9876, 1.5f, FColor::Magenta, FString::Printf(TEXT("Crouching")));
}

void APlayerCharacter::JumpButtonPressed()
{
	if(CombatState >= ECombatState::HangingLedge)
	{
		HandleCombatState(ECombatState::Unoccupied);
		bCanGrabLedge = false;
		GetWorld()->GetTimerManager().SetTimer(OffLedgeTimer, this, &APlayerCharacter::OffLedgeEnd, OffLedgeTime);
	}
	bIsCrouched ? TryVaultingDown() : Jump();
}

void APlayerCharacter::TraceForPlatforms() const
{
	// if(GetCharacterMovement()->IsFalling() || GetVelocity().Z < 0.f) {}
	
	const FVector Start = GroundPoint->GetComponentLocation() + FVector::DownVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FVector End = Start + FVector::DownVector * PlatformTraceDistance;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EOT_OneWayPlatform);
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this, Start, End, ObjectTypes, false, TArray<AActor*>(),
		EDrawDebugTrace::ForOneFrame, Hit, true);
		
	if(Hit.bBlockingHit /*&& GetVelocity().Z < 0.f*/)
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

			// FLinearColor Color = LedgeHit.bBlockingHit ? FLinearColor::Green : FLinearColor::Red;
			// UKismetSystemLibrary::DrawDebugSphere(this, LedgeHit.bBlockingHit ? LedgeHit.Location : LedgeHit.TraceEnd, 5.f, 12, Color, 2.f);
			
			if(LedgeHit.bBlockingHit) break;
		}
		// if(LedgeHit.bBlockingHit)
		// {
		// 	GEngine->AddOnScreenDebugMessage(1654, 5.f, FColor::Green, FString::Printf(TEXT("Has Found Correct Hit: %s"), *LedgeHit.Location.ToString()));
		// }
		// else
		// {
		// 	GEngine->AddOnScreenDebugMessage(1654, 5.f, FColor::Red, FString::Printf(TEXT("Has NOT Found Correct Hit: %s"), *LedgeHit.TraceEnd.ToString()));
		// }
		if(LedgeHit.bBlockingHit && LedgeHit.GetActor() && LedgeHit.GetActor()->ActorHasTag("VaultDownPlatform")) return;
		HandleHangingOnLedge(LedgeHit.bBlockingHit ? LedgeHit.Location - Delta : LedgeHit.TraceEnd - Delta);
	}
}

void APlayerCharacter::TraceForSlope()
{
	if(GetCharacterMovement()->IsFalling())
	{
		const FVector Start = GroundPoint->GetComponentLocation() + FVector::DownVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		const FVector End = Start + FVector::DownVector * PlatformTraceDistance * 1.5f;
		TArray<AActor*> ActorsToIgnore;
		FHitResult Hit;
		UKismetSystemLibrary::LineTraceSingle(
			this, Start, End, TraceTypeQuery1,
			false, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, Hit, true);

		ECombatState NewState = ECombatState::Unoccupied;
		if(Hit.bBlockingHit)
		{
			float Angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(FVector::UpVector, Hit.Normal)));
			// UKismetSystemLibrary::DrawDebugLine(this, Start, Start + Hit.Normal * 50.f, FLinearColor::Red);
			// UKismetSystemLibrary::DrawDebugLine(this, Start, Start + Hit.ImpactNormal * 150.f, FLinearColor::Green);
			// GEngine->AddOnScreenDebugMessage(9874, 2.f, FColor::Cyan, FString::Printf(TEXT("%f"), Angle));
			if (GetCharacterMovement()->GetWalkableFloorAngle() < Angle)
			{
				NewState = ECombatState::OnGroundSlope;
			}
		}
		HandleCombatState(NewState);
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
