// @ Retropsis 2024-2025.

#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterAnimInstance.h"
#include "Player/PlayerCharacterController.h"
#include "Player/PlayerCharacterState.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/Data/AttackSequenceInfo.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/CharacterInfo.h"
#include "AbilitySystem/Effect/StatusEffectNiagaraComponent.h"
#include "Inventory/PlayerInventoryComponent.h"
#include "PaperZDAnimInstance.h"
#include "PaperFlipbookComponent.h"
#include "Interaction/InteractionInterface.h"
#include "Interaction/PlatformInterface.h"
#include "Inventory/HotbarComponent.h"
#include "Inventory/Container/Container.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Data/AbilitySet.h"
#include "Data/InventoryCostData.h"
#include "Engine/AssetManager.h"
#include "Game/LeyrGameMode.h"
#include "Game/LoadMenuSaveGame.h"
#include "Game/BaseGameplayTags.h"
#include "Interaction/ElevatorInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Controller/InventoryWidgetController.h"
#include "UI/PlayerHUD.h"
#include "World/Map/CameraBoundary.h"
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
	InterpCameraAdditiveOffset(DeltaSeconds);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitializeCameraBoundary();
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Init Ability Actor Info Server Side
	InitAbilityActorInfo();
	InitializeCharacterInfo();
	LoadProgress();
	
	if (const ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		LeyrGameMode->LoadWorldState(GetWorld());
	}
	
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
	ParallaxController = GetWorld()->SpawnActor<AParallaxController>(AParallaxController::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
	if(ParallaxController)
	{
		//TODO: Here should be pulled from data asset and not hard coded
		ParallaxController->CurrentMapName = FName("Dorn");
		ParallaxController->InitializeMapParallax();
		ParallaxController->AttachToComponent(FollowCamera, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void APlayerCharacter::InitializeCameraBoundary()
{
	TArray<AActor*> OutActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(ObjectTypeQuery2);
	UKismetSystemLibrary::BoxOverlapActors(this, GetActorLocation(), FVector{50.f }, ObjectTypes, ACameraBoundary::StaticClass(), ActorsToIgnore, OutActors);
	for (AActor* Actor : OutActors)
	{
		if (ACameraBoundary* CameraBoundary = Cast<ACameraBoundary>(Actor))
		{
			CameraBoundary->HandleOnBeginOverlap(this);
			break;
		}
	}
}

void APlayerCharacter::ClampToCameraBounds(FVector& PreferredCameraLocation) const
{
	if (CameraBounds.bInitialized)
	{
		if (PreferredCameraLocation.X < CameraBounds.Left.X || PreferredCameraLocation.X > CameraBounds.Right.X)
		{
			PreferredCameraLocation.X = FMath::Clamp(PreferredCameraLocation.X, CameraBounds.Left.X, CameraBounds.Right.X);
		}
		if (PreferredCameraLocation.Z < CameraBounds.Bottom.Z || PreferredCameraLocation.Z > CameraBounds.Top.Z)
		{
			PreferredCameraLocation.Z = FMath::Clamp(PreferredCameraLocation.Z, CameraBounds.Bottom.Z, CameraBounds.Top.Z);
		}
	}
}

bool APlayerCharacter::IsWithinCameraBounds(const FVector& InLocation) const
{
	return CameraEnteringBounds.GetBox().IsValid ? CameraEnteringBounds.GetBox().IsInside(InLocation) : false;
}

void APlayerCharacter::InterpCameraAdditiveOffset(float DeltaTime)
{
	// Cache Initial Values
	FTransform AdditiveOffset;
	float FOV = FollowCamera->FieldOfView;
	float InterpSpeed = 10.f;
	FollowCamera->GetAdditiveOffset(AdditiveOffset, FOV);
	bool bClampFirst = true;

	const FVector CameraLocation = FollowCamera->GetComponentLocation();
	FVector PreferredCameraLocation = GetActorLocation();

	// Adjusting Preferred Location for crouched state
	PreferredCameraLocation.Z += bIsCrouched ? GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - GetCharacterMovement()->GetCrouchedHalfHeight() : 0.f;

	switch (CameraInterpState)
	{
	case ECameraInterpState::None: return;
	case ECameraInterpState::WithinBounds:
		bClampFirst = false;
		break;
	case ECameraInterpState::Entering:	
		InterpSpeed = EnteringInterpSpeed;
		break;
	case ECameraInterpState::Leaving:
		InterpSpeed = ExitingInterpSpeed;
		break;
	case ECameraInterpState::Following:
		PreferredCameraLocation = GetActorLocation();
		PreferredCameraLocation.Z += bIsCrouched ? GetCapsuleComponent()->GetScaledCapsuleHalfHeight() : 0.f;
		
		// const FRealCurve* InterpSpeedCurve = ScalableInterpSpeedCurve->FindCurve(FName("InterpSpeedCurve"), FString());
		// InterpSpeed = InterpSpeedCurve->Eval((PreferredCameraLocation - CameraLocation).Size());
		
		bClampFirst = false;
		if (ActorToInterp && GetDistanceTo(ActorToInterp) <= ActorToFollowMaxDistance)
		{
			if (!IsWithinCameraBounds(LastActorToFollowInterpLocation)) LastActorToFollowInterpLocation = FVector{ (ActorToInterp->GetActorLocation() + GetActorLocation()) / 2.f };
			PreferredCameraLocation = FMath::VInterpTo(LastActorToFollowInterpLocation, FVector{ (ActorToInterp->GetActorLocation() + GetActorLocation()) / 2.f }, DeltaTime, InterpSpeed);
			LastActorToFollowInterpLocation = PreferredCameraLocation;
			InterpSpeed = FollowingInterpSpeed;
			bClampFirst = false;
		}
		else if (ActorToInterp && GetDistanceTo(ActorToInterp) > ActorToFollowMaxDistance)
		{
			if (!IsWithinCameraBounds(LastActorToFollowInterpLocation)) LastActorToFollowInterpLocation = PreferredCameraLocation;
			PreferredCameraLocation = FMath::VInterpTo(LastActorToFollowInterpLocation, PreferredCameraLocation, DeltaTime, 1.0f);
			LastActorToFollowInterpLocation = PreferredCameraLocation;
		}
		break;
	}
	UKismetSystemLibrary::DrawDebugSphere(this, PreferredCameraLocation, 25.f, 12, FLinearColor::White);
	UKismetSystemLibrary::DrawDebugSphere(this, LastActorToFollowInterpLocation, 23.f, 12, FLinearColor::Red);
	
	// Clamp Camera to Bounds First
	if(bClampFirst)
	{
		ClampToCameraBounds(PreferredCameraLocation);
	}
	
	// Interp camera between current location toward preferred location
	FollowCamera->ClearAdditiveOffset();
	FVector Delta = PreferredCameraLocation - CameraLocation;
	FVector TargetAdditiveOffset = FVector{0.f, Delta.X, Delta.Z};
	
	CurrentAdditiveOffset = FMath::VInterpTo(AdditiveOffset.GetLocation(), TargetAdditiveOffset, DeltaTime, InterpSpeed);
	AdditiveOffset.SetLocation(CurrentAdditiveOffset);
	FollowCamera->AddAdditiveOffset(AdditiveOffset, FOV);
	ParallaxController->SetActorRelativeLocation(FVector{ 600.f, CurrentAdditiveOffset.Y, CurrentAdditiveOffset.Z });

	// Clamp Camera to Bounds After
	if(!bClampFirst)
	{
		ClampToCameraBounds(PreferredCameraLocation);
		// if (ActorToInterp)
		// {
		// 	UKismetSystemLibrary::DrawDebugSphere(this, FVector{ (ActorToInterp->GetActorLocation() + GetActorLocation()) / 2.f }, 25.f, 12, FLinearColor::White);
		// 	UKismetSystemLibrary::DrawDebugSphere(this, PreferredCameraLocation, 25.f, 12, FLinearColor::Green);
		// }

		FollowCamera->ClearAdditiveOffset();
		Delta = PreferredCameraLocation - CameraLocation;
		TargetAdditiveOffset = FVector{ 0.f, Delta.X, Delta.Z };
		AdditiveOffset.SetLocation(TargetAdditiveOffset);
		FollowCamera->AddAdditiveOffset(AdditiveOffset, FOV);
		ParallaxController->SetActorRelativeLocation(FVector{ 600.f, TargetAdditiveOffset.Y, TargetAdditiveOffset.Z });
	}

	// Check State
	if (FVector::PointsAreNear(CurrentAdditiveOffset, TargetAdditiveOffset, SnapDistance))
	{
		if (CameraInterpState == ECameraInterpState::Entering) 	CameraInterpState = ECameraInterpState::Following;
		
		if (!CameraBounds.bInitialized) FollowCamera->ClearAdditiveOffset();
	}
	PreviousAdditiveOffset = AdditiveOffset.GetLocation();
}

FVector APlayerCharacter::GetCameraOffsetLocation() const
{
	FTransform Transform;
	float FOV;
	FollowCamera->GetAdditiveOffset(Transform, FOV);
	return FollowCamera->GetComponentLocation() + FVector{ Transform.GetLocation().Y, 600.f, 0.f };
}

void APlayerCharacter::SetCameraInterpolation_Implementation(ACameraBoundary* CameraBoundary, ECameraInterpState NewState)
{
	if(NewState == ECameraInterpState::Leaving && CurrentCameraBoundary != CameraBoundary) return;
	if(CameraBoundary)
	{
		CurrentCameraBoundary = CameraBoundary;
		CameraBounds = FCameraBounds{ CameraBoundary->GetBounds(), FollowCamera->GetComponentLocation() };
		CameraBounds.bInitialized = NewState == ECameraInterpState::Following || NewState == ECameraInterpState::Entering;
		ActorToInterp = CameraBoundary->GetTargetActor();
		CameraEnteringBounds = CameraBoundary->GetEnteringBounds();
		CameraInterpState = NewState;
		CurrentAdditiveOffset = FVector::ZeroVector;
	}
}

void APlayerCharacter::DisableCameraLagForDuration_Implementation(float Duration)
{
	SpringArm->bEnableCameraLag = false;
	FTimerHandle DisableCameraLagTimer;
	GetWorld()->GetTimerManager().SetTimer(DisableCameraLagTimer, FTimerDelegate::CreateLambda([this](){ SpringArm->bEnableCameraLag = true; }), Duration, false);		
}

void APlayerCharacter::ServerInteract_Implementation()
{	
	if(CombatState == ECombatState::HangingLadder ||
		CombatState == ECombatState::Climbing ||
		GetCharacterMovement()->IsFalling()) return;

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

	// if(APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(GetController()))
	// {
	// 	if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerCharacterController->GetHUD()))
	// 	{
	// 		PlayerHUD->InitOverlay(PlayerCharacterController, PlayerCharacterState, AbilitySystemComponent, AttributeSet);
	// 	}
	// }

	//TODO: Move this to some other (PlayerState, BeginPlay)
	FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.StatusEffect_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::HitReactTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.StatusEffect_Weakened, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::WeakenedTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.VisualEffect_HitReactFlash, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::HitReactFlashTagChanged);
 	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.StatusEffect_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::StunTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.StatusEffect_Burn, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::BurnTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.CombatState_Condition_Swimming, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::SwimmingTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.Peaceful, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::PeacefulTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.CombatState_Rule, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::RuleTagChanged);
}

void APlayerCharacter::InitializeCharacterInfo()
{
	if(!HasAuthority()) return;
	
	const FCharacterDefaultInfo Info = CharacterInfo->GetCharacterDefaultInfo(CharacterTag);
	
	for (TTuple<FGameplayTag, TSoftObjectPtr<UPaperZDAnimSequence>> HitReactSequence :  Info.HitReactSequences)
	{
		if (!HitReactSequence.Key.IsValid()) continue;
		
		UAssetManager::GetStreamableManager().RequestAsyncLoad(HitReactSequence.Value.ToSoftObjectPath(), [this, HitReactSequence] () {
			UPaperZDAnimSequence* LoadedAsset = HitReactSequence.Value.Get();
			if (IsValid(LoadedAsset))
			{
				HitReactSequences.Add(HitReactSequence.Key, LoadedAsset);
			}
		}, FStreamableManager::DefaultAsyncLoadPriority);
	}
	
	for (TTuple<FGameplayTag, TSoftObjectPtr<UNiagaraSystem>> WoundImpactEffect :  Info.WoundImpactEffects)
	{
		if (!WoundImpactEffect.Key.IsValid()) continue;
		
		UAssetManager::GetStreamableManager().RequestAsyncLoad(WoundImpactEffect.Value.ToSoftObjectPath(), [this, WoundImpactEffect] () {
			UNiagaraSystem* LoadedAsset = WoundImpactEffect.Value.Get();
			if (IsValid(LoadedAsset))
			{
				WoundImpactEffects.Add(WoundImpactEffect.Key, LoadedAsset);
			}
		}, FStreamableManager::DefaultAsyncLoadPriority);
	}
	
	DefeatedSoundLoaded = Info.DeathSound;
	AttackSequenceInfo = Info.AttackSequenceInfo;
}

void APlayerCharacter::AddCharacterAbilities()
{
	if (!HasAuthority()) return;
	
	checkf(CharacterInfo, TEXT("CharacterInfo is missing on [%s]"), *GetName());
	checkf(CharacterInfo->AbilitySet, TEXT("AbilitySet is missing on [%s]"), *CharacterInfo->GetName());

	UBaseAbilitySystemComponent* BaseASC = CastChecked<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	CharacterInfo->AbilitySet->GiveToAbilitySystem(BaseASC, &GrantedHandles, FGameplayTag(), GetPlayerLevel(), BaseASC);
}

// TODO: Need a less hardcoded way to do, here we suppose first 3 indices are the 3 combos
FTaggedMontage APlayerCharacter::GetTaggedMontageByIndex_Implementation(int32 Index)
{
	return AttackSequenceInfo->OneHandedSequences.IsValidIndex(Index) ? AttackSequenceInfo->OneHandedSequences[Index] : FTaggedMontage();
}

void APlayerCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	if(bHitReacting)
	{
		AbilitySystemComponent->AddLooseGameplayTags(GameplayTags.CombatState_Rule_Block_Movement.GetSingleTagContainer());
	}
	else
	{
		AbilitySystemComponent->RemoveLooseGameplayTags(GameplayTags.CombatState_Rule_Block_Movement.GetSingleTagContainer());
	}
}

void APlayerCharacter::PeacefulTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		HandleCombatState(ECombatState::Peaceful, FCombatStateParams());
	}
	else
	{
		FCombatStateParams Params;
		Params.bResetMovementMode = false;
		HandleCombatState(ECombatState::Unoccupied, Params);
	}
}

void APlayerCharacter::SwimmingTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Cyan, "SwimmingTagChanged");
		// TODO : Spawn Entering Splashes and/or bubbles
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Cyan, "SwimmingFinished");
		// TODO : Spawn Leaving Splashes and/or bubbles
	}
}

void APlayerCharacter::RuleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	if (AbilitySystemComponent->HasMatchingGameplayTag(GameplayTags.CombatState_Rule_Block_Movement))
	{
		GetCharacterMovement()->StopMovementImmediately();
	}
	if (AbilitySystemComponent->HasMatchingGameplayTag(GameplayTags.CombatState_Rule_Stop_Movement))
	{
		GetCharacterMovement()->StopMovementImmediately();
	}
}

/*
 * Movement
 */
void APlayerCharacter::Move(const FVector2D MovementVector)
{
	FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	if (AbilitySystemComponent->HasMatchingGameplayTag(GameplayTags.CombatState_Rule_Block_Movement)) return;

	// TODO: Remove this line when Root Motion Abilities are handling rules
	if(CombatState >= ECombatState::Dodging) return;
	
	if(CombatState < ECombatState::Aiming) RotateController();
	
	PreviousCombatDirection = CombatDirection;
	CombatDirection = GetCombatDirectionFromVector2D(MovementVector);
	if(PreviousCombatDirection != CombatDirection) HandleCombatDirectionTag();
	
	HandleCrouching(MovementVector.Y < 0.f);
	
	switch (CombatState) {
	case ECombatState::Unoccupied:
	case ECombatState::Falling:
	case ECombatState::Crouching:
		AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		if (GetCharacterMovement()->MovementMode == MOVE_Falling && AbilitySystemComponent->GetTagCount(FBaseGameplayTags::Get().CombatState_Condition_Falling) == 0)
		{
			MakeAndApplyEffectToSelf(FBaseGameplayTags::Get().CombatState_Condition_Falling);
		}
		break;
	case ECombatState::Walking:
	case ECombatState::Peaceful:
		AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		break;
	case ECombatState::Aiming:
	case ECombatState::Casting:
	case ECombatState::Shielding:
		AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		Pitch(MovementVector.Y);
		break;
	case ECombatState::HangingRope:
		AddMovementInput(FVector(1.f, 0.f, 0.f), FMath::RoundToFloat(MovementVector.X));
		TryClimbingRope(MovementVector);
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
	case ECombatState::ClimbingRope:
	case ECombatState::HoppingLedge:
	case ECombatState::OnGroundSlope:
	case ECombatState::OnRopeSlope:
	case ECombatState::Dodging:
	case ECombatState::Rolling:
	case ECombatState::RollingEnd:
	case ECombatState::Stunned:
	case ECombatState::Defeated:
		break;
	}
}

void APlayerCharacter::TryClimbingRope(const FVector2D MovementVector)
{
	if(MovementVector.Y <= 0.f) return;

	const FVector Start = GetActorLocation() + GetActorUpVector() * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FVector End = Start + GetActorUpVector() * (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f +10.f);
	FHitResult Hit;
	const float Radius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(ObjectTypeQuery1);
	UKismetSystemLibrary::SphereTraceSingleForObjects(this, Start, End, Radius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true);

	if (!Hit.bBlockingHit)
	{
		HandleCombatState(ECombatState::ClimbingRope);
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

void APlayerCharacter::OnRep_Stunned()
{
	if (UBaseAbilitySystemComponent* AuraASC = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		if (bIsStunned)
		{
			AuraASC->AddLooseGameplayTags(BlockedTags);
			StunStatusEffectComponent->Activate();
		}
		else
		{
			AuraASC->RemoveLooseGameplayTags(BlockedTags);
			StunStatusEffectComponent->Deactivate();
		}
	}
}

void APlayerCharacter::OnRep_Burned()
{	
	if (bIsBurned)
	{
		BurnStatusEffectComponent->Activate();
	}
	else
	{
		BurnStatusEffectComponent->Deactivate();
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

bool APlayerCharacter::TryDescendLadder()
{
	const FVector Start = GetActorLocation() + FVector::DownVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FVector End = Start + FVector::DownVector * 15.f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EOT_OneWayPlatform);
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this, Start, End, ObjectTypes,
		false, TArray<AActor*>(), EDrawDebugTrace::None, Hit, true, FLinearColor::Yellow);
	if (Hit.bBlockingHit && Hit.GetActor() && Hit.GetActor()->Implements<UInteractionInterface>())
	{
		IInteractionInterface::Execute_InteractBottom(Hit.GetActor(), this);
		return true;
	}
	return false;
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
		AbilitySystemComponent->TryActivateAbilitiesByTag(FBaseGameplayTags::Get().Abilities_SwimImpulse.GetSingleTagContainer());
		return;
	}
	if(CombatState >= ECombatState::HangingLedge)
	{
		HandleCombatState(ECombatState::Unoccupied);
		bCanGrabLedge = false;
		GetWorld()->GetTimerManager().SetTimer(OffLedgeTimer, FTimerDelegate::CreateLambda([this] () { bCanGrabLedge = true; }), OffLedgeTime, false);
		if(GetAttachParentActor()) DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	if (bIsCrouched)
	{
		if (!TryVaultingDown())
		{
			if (AbilitySystemComponent->TryActivateAbilitiesByTag(FBaseGameplayTags::Get().Abilities_Roll.GetSingleTagContainer()))
			{
				HandleCombatState(ECombatState::Rolling);
			}
		}
	} else Jump();
}

void APlayerCharacter::SetCombatStateToHandle_Implementation(ECombatState NewState, const FCombatStateParams& Params)
{	
	HandleCombatState(NewState, Params);
}

void APlayerCharacter::HandleCombatState(ECombatState NewState, const FCombatStateParams& Params)
{
	CombatState = NewState;
	GetCharacterMovement()->GravityScale = BaseGravityScale;
	if(Params.bResetCollisionResponse)
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Enemy, ECR_Overlap);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(GameplayTags.CombatStates);
	
	switch (CombatState) {
	case ECombatState::Unoccupied:
		GetCharacterMovement()->MaxWalkSpeed = BaseRunSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = BaseWalkSpeedCrouched;
		GetCharacterMovement()->MaxFlySpeed = BaseFlySpeed;
		GetCharacterMovement()->GravityScale = BaseGravityScale;
		GetCharacterMovement()->MaxAcceleration = 2048.f;
		GetCharacterMovement()->BrakingFrictionFactor = 2.f;
		if (Params.bResetMovementMode) GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		break;
	case ECombatState::Falling:
		GetCharacterMovement()->MaxWalkSpeed = BaseRunSpeed;
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		break;
	case ECombatState::Walking:
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Condition_Walking);
		break;
	case ECombatState::Peaceful:
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
		GetSprite()->SetRelativeLocation(FVector::ZeroVector);
		CombatState = ECombatState::Unoccupied;
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
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Elevator);
		break;
	case ECombatState::OnGroundSlope:
		GetCharacterMovement()->GravityScale = GroundSlopeGravityScale;
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Condition_Slope);
		break;
	case ECombatState::OnRopeSlope:
		break;
	case ECombatState::Entangled:
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Condition_Entangled);
		break;
	case ECombatState::Swimming:
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->MaxFlySpeed = SwimmingSpeed;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Condition_Swimming);
		break;
	case ECombatState::ClimbingRope:
		AbilitySystemComponent->TryActivateAbilitiesByTag(GameplayTags.Abilities_RootMotion.GetSingleTagContainer());
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Transient_Rope);
		break;
	case ECombatState::HoppingLedge:
		AbilitySystemComponent->TryActivateAbilitiesByTag(GameplayTags.Abilities_RootMotion.GetSingleTagContainer());
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
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Enemy, ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore);
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Transient_Dodging);
		break;
	case ECombatState::Rolling:
		Crouch();
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Transient_Rolling);
		break;
	case ECombatState::RollingEnd:
		GetSprite()->SetRelativeLocation(FVector::ZeroVector);
		UnCrouch();
		GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(GameplayTags.CombatState_Directional_Downward.GetSingleTagContainer());
		HandleCombatState(bIsCrouched ? ECombatState::Crouching : ECombatState::Unoccupied);
		break;
	case ECombatState::Aiming:
	case ECombatState::Casting:
	case ECombatState::Shielding:
		GetCharacterMovement()->MaxWalkSpeed = AimingWalkSpeed;
		MakeAndApplyEffectToSelf(GameplayTags.CombatState_Transient_Aiming);
		break;
	case ECombatState::Defeated:
		MakeAndApplyEffectToSelf(FBaseGameplayTags::Get().CombatState_Defeated);
		break;
	case ECombatState::Stunned:
		break;
	}
	// UE_LOG(LogTemp, Warning, TEXT("CombatState: %s"), *UEnum::GetValueAsString(CombatState));
	// UE_LOG(LogTemp, Warning, TEXT("MovementMode: %s"), *UEnum::GetValueAsString(GetCharacterMovement()->MovementMode));
}

void APlayerCharacter::HandleCharacterMovementUpdated(float DeltaSeconds, FVector OldLocation, FVector OldVelocity)
{	
	if(GetCharacterMovement()->MovementMode == PreviousMovementMode) return;
	
	PreviousMovementMode = GetCharacterMovement()->MovementMode;
	if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		MakeAndApplyEffectToSelf(FBaseGameplayTags::Get().CombatState_Condition_Falling);
	}
	else
	{
		GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FBaseGameplayTags::Get().CombatState_Condition_Falling.GetSingleTagContainer());
	}
}

void APlayerCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void APlayerCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	HandleCombatState(ECombatState::UnCrouching);
}

void APlayerCharacter::HandleCrouching(bool bShouldCrouch)
{
	bCrouchButtonHeld = bShouldCrouch;
	if(CombatState > ECombatState::Falling) return;
	
	if(!bShouldCrouch)
	{
		UnCrouch();
	}
	if(GetCharacterMovement()->IsFalling())
	{
		UnCrouch();
		return;
	}
	if((bShouldCrouch && bIsCrouched) || CombatState >= ECombatState::HangingLedge) return;
	if(bShouldCrouch && !bIsCrouched && CombatState == ECombatState::Unoccupied)
	{
		TryDescendLadder();
		HandleCombatState(ECombatState::Crouching);
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
		CurrentMinZ = MinZ;
        GetCharacterMovement()->SetMovementMode(MOVE_Falling);
        GetCharacterMovement()->MaxFlySpeed = EntangledWalkSpeed;
        GetCharacterMovement()->GravityScale = EntangledGravityScale;
        HandleCombatState(ECombatState::Entangled);
	}
}

/*
 * Swimming
 */
void APlayerCharacter::HandleSwimming_Implementation(float MinZ, float EnvironmentSwimmingSpeed, float SwimmingGravityScale, bool bEndOverlap)
{
	if(bEndOverlap)
	{
		GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = false;
		HandleCombatState(ECombatState::Unoccupied);
		Jump();
	}
	else
	{
		CurrentMinZ = MinZ;
		GetCharacterMovement()->MaxSwimSpeed = SwimmingSpeed;
		GetCharacterMovement()->GravityScale = SwimmingGravityScale;
		HandleCombatState(ECombatState::Swimming);
	}
}

/*
 * Elevator
 */
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

void APlayerCharacter::ResetAimingPitch_Implementation()
{
	if(ForgetOverridePitchTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ForgetOverridePitchTimer);
	}
	OverridePitch = 0.f;
	UpperBody->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
}

/*
 * Saving / Loading
 */
void APlayerCharacter::SaveProgress_Implementation(const FName& SavePointTag)
{
	if (const ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		ULoadMenuSaveGame* SaveData = LeyrGameMode->RetrieveInGameSaveData();
		if(SaveData == nullptr) return;

		SaveData->PlayerStartTag = SavePointTag;

		if (APlayerCharacterState* PlayerCharacterState = Cast<APlayerCharacterState>(GetPlayerState()))
		{
			SaveData->Level = PlayerCharacterState->GetCharacterLevel();
			SaveData->Experience = PlayerCharacterState->GetXP();
			SaveData->SkillPoints = PlayerCharacterState->GetSkillPoints();
			SaveData->AttributePoints = PlayerCharacterState->GetAttributePoints();
		}		
		SaveData->Strength = UBaseAttributeSet::GetStrengthAttribute().GetGameplayAttributeData(GetAttributeSet())->GetBaseValue();
		SaveData->Dexterity = UBaseAttributeSet::GetDexterityAttribute().GetGameplayAttributeData(GetAttributeSet())->GetBaseValue();
		SaveData->Vitality = UBaseAttributeSet::GetVitalityAttribute().GetGameplayAttributeData(GetAttributeSet())->GetBaseValue();
		SaveData->Intelligence = UBaseAttributeSet::GetIntelligenceAttribute().GetGameplayAttributeData(GetAttributeSet())->GetBaseValue();
		SaveData->Wisdom = UBaseAttributeSet::GetWisdomAttribute().GetGameplayAttributeData(GetAttributeSet())->GetBaseValue();
		SaveData->Spirit = UBaseAttributeSet::GetSpiritAttribute().GetGameplayAttributeData(GetAttributeSet())->GetBaseValue();
		
		

		SaveData->bFirstTimeLoadIn = false;

		if(!HasAuthority()) return;

		SaveData->SavedItems = PlayerInventory->Items;
	
		if(APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(Controller))
		{
			if(UInventoryWidgetController* IC =PlayerCharacterController->GetInventoryWidgetController())
			{
				SaveData->SavedEquippedItems = IC->GetEquippedItems();
			}
		}

		UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent);
		FForEachAbility SaveAbilityDelegate;
		SaveData->SavedAbilities.Empty();
		SaveAbilityDelegate.BindLambda([this, BaseASC, SaveData] (const FGameplayAbilitySpec& AbilitySpec)
		{
			const FGameplayTag AbilityTag = BaseASC->GetAbilityTagFromSpec(AbilitySpec);
			const UAbilityInfo* AbilityInfo = ULeyrAbilitySystemLibrary::GetAbilityInfo(this);
			const FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			
			if (!Info.bSerialize) return;
			
			FAbilitySaveData AbilitySaveData;
			AbilitySaveData.GameplayAbility = Info.Ability;
			AbilitySaveData.AbilityLevel = AbilitySpec.Level;
			AbilitySaveData.AbilityInput = BaseASC->GetInputTagFromAbilityTag(AbilityTag);
			AbilitySaveData.AbilityStatus = BaseASC->GetStatusFromAbilityTag(AbilityTag);
			AbilitySaveData.AbilityTag = AbilityTag;
			AbilitySaveData.AbilityType = Info.AbilityType;

			SaveData->SavedAbilities.AddUnique(AbilitySaveData);
		});
		BaseASC->ForEachAbility(SaveAbilityDelegate);
		
		LeyrGameMode->SaveInGameProgressData(SaveData);
	}
}

void APlayerCharacter::LoadProgress()
{
	if (const ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		ULoadMenuSaveGame* SaveData = LeyrGameMode->RetrieveInGameSaveData();
		if(SaveData == nullptr) return;
		
		// InitializeDefaultAttributes(); // Done along AbilitySet->GiveToAbilitySystem
		AddCharacterAbilities();
		if (!SaveData->bFirstTimeLoadIn)
		{
			if (UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent))
			{
				BaseASC->AddCharacterAbilitiesFromSaveData(SaveData);
			}			
			if (APlayerCharacterState* PlayerCharacterState = Cast<APlayerCharacterState>(GetPlayerState()))
			{
				PlayerCharacterState->SetLevel(SaveData->Level);
				PlayerCharacterState->SetXP(SaveData->Experience);
				PlayerCharacterState->SetSkillPoints(SaveData->SkillPoints);
				PlayerCharacterState->SetAttributePoints(SaveData->AttributePoints);
			}
			ULeyrAbilitySystemLibrary::InitializeCharacterAttributesFromSaveData(this, GetAbilitySystemComponent(), SaveData);

			PlayerInventory->Items = SaveData->SavedItems;
		}
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
	
	if(Hit.GetComponent() && Hit.GetComponent()->ComponentHasTag("Rope") && GetVelocity().Z < -10.f && CombatState != ECombatState::ClimbingRope)
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
	if(!bCanGrabLedge || !GetCharacterMovement()->IsFalling()) return;
	
	TArray<AActor*> ActorsToIgnore;
	FHitResult BottomHit;
	FHitResult MidHit;
	FHitResult TopHit;
	const FVector Delta = FVector(GetCapsuleComponent()->GetScaledCapsuleRadius(), 0.f, 0.f) * GetActorForwardVector().X;

	const FVector BottomStart = GetActorLocation() - GetActorUpVector() * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FVector BottomEnd = BottomStart - GetActorUpVector() * 25.f;
	UKismetSystemLibrary::LineTraceSingle(this, BottomStart, BottomEnd, TraceTypeQuery1,
	false, ActorsToIgnore, EDrawDebugTrace::None, BottomHit, true);
	
	UKismetSystemLibrary::LineTraceSingle(this, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 45.f, TraceTypeQuery1,
	false, ActorsToIgnore, EDrawDebugTrace::None, MidHit, true);
	
	UKismetSystemLibrary::LineTraceSingle(this, RopeHangingCollision->GetComponentLocation(), RopeHangingCollision->GetComponentLocation() + RopeHangingCollision->GetForwardVector() * 45.f, TraceTypeQuery1,
		false, ActorsToIgnore, EDrawDebugTrace::None, TopHit, true);

	if(MidHit.bBlockingHit && MidHit.GetActor() && MidHit.GetActor()->ActorHasTag("Platform")) return;

	if(MidHit.bBlockingHit && !TopHit.bBlockingHit && !BottomHit.bBlockingHit)
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

bool APlayerCharacter::TryVaultingDown()
{
	if(GetCharacterMovement()->IsFalling() || GetVelocity().Z < 0.f) return false;
	
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
		return true;
	}
	return false;
}

void APlayerCharacter::TraceForSlope()
{
	if(CombatState == ECombatState::OnElevator || CombatState == ECombatState::Peaceful) return;
	
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
	IControllerInterface::Execute_ToggleContainer(Controller, true, Container->GetSlotCountContainer());
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
	case EContainerType::Use:
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

void APlayerCharacter::TryOpenKeylock_Implementation(const TSoftObjectPtr<UItemData>& Asset)
{
	for (FInventoryItemData Item : PlayerInventory->Items)
	{
		if (Item.Asset == Asset && Asset.IsValid())
		{
			OnKeyItemUsed.Broadcast();
			return;
		}
	}
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

int32 APlayerCharacter::FindXPForLevel_Implementation(int32 InLevel) const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->LevelUpInfo->FindXPForLevel(InLevel);
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

int32 APlayerCharacter::GetPlayerLevel() const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->GetCharacterLevel();
}

void APlayerCharacter::SetCombatState_Implementation(ECombatState NewState)
{
	CombatState = NewState;
}

void APlayerCharacter::Die(const FVector& DeathImpulse, bool bExecute)
{
	Super::Die(DeathImpulse, bExecute);
	PreviousCombatState = CombatState;
	HandleCombatState(ECombatState::Defeated);
	if (PreviousCombatState == ECombatState::Swimming) GetCharacterMovement()->GravityScale = 0.f;
	if (PreviousCombatState == ECombatState::Entangled) GetCharacterMovement()->GravityScale = 0.f;

	FTimerDelegate DefeatTimerDelegate;
	DefeatTimerDelegate.BindLambda([this] ()
	{
		if (ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			// LeyrGameMode->PlayerDefeated(this);
			if (APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(Controller))
			{
				if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerCharacterController->GetHUD()))
				{
					if(IsValid(PlayerHUD)) PlayerHUD->PlayerDefeated();
				}
			}
		}
	});
	GetWorldTimerManager().SetTimer(DefeatTimer, DefeatTimerDelegate, DefeatTime, false);
	
	FollowCamera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}