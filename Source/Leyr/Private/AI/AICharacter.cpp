// @ Retropsis 2024-2025.

#include "AI/AICharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystem/Data/EncounterInfo.h"
#include "AI/AICharacterAnimInstance.h"
#include "AI/BaseAIController.h"
#include "AI/SplineComponentActor.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/BaseGameplayTags.h"
#include "Game/LeyrGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "World/Level/Zone/Arena.h"

AAICharacter::AAICharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);
	
	PassiveIndicatorComponent = CreateDefaultSubobject<UWidgetComponent>("PassiveIndicator");
	PassiveIndicatorComponent->SetupAttachment(GetRootComponent());
	PassiveIndicatorComponent->SetWidgetSpace(EWidgetSpace::Screen);
}

#if WITH_EDITOR
void AAICharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if(MovementType == EMovementType::Spline && !SplineComponentActor)
	{
		FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SplineComponentActor = GetWorld()->SpawnActor<ASplineComponentActor>(ASplineComponentActor::StaticClass(), GetActorTransform(), SpawnParameters);
		SplineComponentActor->SetActorLabel(FString::Printf(TEXT("%s_PatrolSpline"), *GetName()));
		for (int i = 0; i < SplineComponentActor->GetSplineComponent()->GetNumberOfSplinePoints(); ++i)
		{
			SplineComponentActor->GetSplineComponent()->SetSplinePointType(i, ESplinePointType::Linear);
		}
	}
	if(MovementType != EMovementType::Spline && SplineComponentActor)
	{
		SplineComponentActor->Destroy();
		SplineComponentActor = nullptr;
	}
	if(!NavMeshBoundsVolume && bShouldBuildNavMesh)
	{
		FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		NavMeshBoundsVolume = GetWorld()->SpawnActor<ANavMeshBoundsVolume>(ANavMeshBoundsVolume::StaticClass(), GetActorTransform(), SpawnParameters);
		NavMeshBoundsVolume->SetActorLabel(FString::Printf(TEXT("%s_NavMesh"), *GetName()));
		// UBrushComponent* Brush = NavMeshBoundsVolume->GetBrushComponent();
		// Brush->Brush->Bounds = FBox(FVector(100.f), FVector(200.f));
		// GetWorld()->GetNavigationSystem()->OnNavigationBoundsUpdated(NavMeshBoundsVolume);

		NavMeshBoundsVolume->GetRootComponent()->Bounds = FBox(FVector(100.f), FVector(200.f));
		NavMeshBoundsVolume->SetActorLocation(FVector{ GetActorLocation().X, 50.f, GetActorLocation().Z });
		// GetWorld()->GetNavigationSystem()->OnNavAreaRegisteredDelegate(NavMeshBoundsVolume);
	}
	if(!bShouldBuildNavMesh && NavMeshBoundsVolume)
	{
		NavMeshBoundsVolume->Destroy();
		NavMeshBoundsVolume = nullptr;
	}
}
#endif

void AAICharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// TODO: Might not need to be in Tick
	if(BaseAIController) BaseAIController->GetBlackboardComponent()->SetValueAsEnum(FName("BehaviourState"), static_cast<uint8>(BehaviourState));
}

void AAICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!HasAuthority()) return;
	InitializeCharacterInfo();

	StartLocation = GetActorLocation();
	
	BaseAIController = Cast<ABaseAIController>(NewController);
	BaseAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	BaseAIController->RunBehaviorTree(BehaviorTree);
	BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
	BaseAIController->GetBlackboardComponent()->SetValueAsVector(FName("StartLocation"), StartLocation);
	BaseAIController->SetPawn(this);

	if (Arena)
	{
		Arena->OnPlayerEntering.AddLambda([this](AActor* Player) { HandlePlayerOverlappingArena(Player, true); });
		Arena->OnPlayerLeaving.AddLambda([this](AActor* Player) { HandlePlayerOverlappingArena(Player, false); });
		Arena->SetTargetActor(this);
	}

	if(bCollisionCauseDamage)
	{
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnBeginOverlap);
	}
	SineMoveHeight = GetActorLocation().Z;
	InitializeNavigationBounds();
	
	switch (BehaviourType) {
	case EBehaviourType::Patrol:
		break;
	case EBehaviourType::Ranger:
		break;
	case EBehaviourType::Turret:
		GetCharacterMovement()->SetMovementMode(MOVE_None);
		GetCharacterMovement()->DefaultLandMovementMode = MOVE_None;
		break;
	case EBehaviourType::Airborne:
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
		break;
	case EBehaviourType::Aquatic:
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
		break;
	}
}

void AAICharacter::InitializeNavigationBounds()
{
	const FVector Start = GetActorLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::CapsuleOverlapActors(this, Start, GetCapsuleComponent()->GetScaledCapsuleRadius(), GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
											   ObjectTypes, ACameraBoundary::StaticClass(), ActorsToIgnore, OutActors);
	if (OutActors.Num() > 0)
	{
		if (const ACameraBoundary* CameraBoundary = Cast<ACameraBoundary>(OutActors[0]))
		{
			EnteringBounds = CameraBoundary->GetEnteringBounds(); 
			NavigationBounds = CameraBoundary->GetNavigationBounds(); 
		}
	}
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	GetCharacterMovement()->GravityScale = BaseGravityScale;
	InitAbilityActorInfo();
	AddAICharacterAbilities();
	
	if (UBaseUserWidget* BaseUserWidget = Cast<UBaseUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		BaseUserWidget->SetWidgetController(this);
	}
	if (UBaseUserWidget* BaseUserWidget = Cast<UBaseUserWidget>(PassiveIndicatorComponent->GetUserWidgetObject()))
	{
		BaseUserWidget->SetWidgetController(this);
	}

	if (const UBaseAttributeSet* BaseAS = Cast<UBaseAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAS->GetHealthAttribute()).AddLambda(
			[this] (const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAS->GetMaxHealthAttribute()).AddLambda(
			[this] (const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->RegisterGameplayTagEvent(FBaseGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAICharacter::HitReactTagChanged);
		AbilitySystemComponent->RegisterGameplayTagEvent(FBaseGameplayTags::Get().Indicator_Execute, EGameplayTagEventType::NewOrRemoved).AddLambda([this] (const FGameplayTag CallbackTag, int32 NewCount)
		{
			OnGameplayTagAddedOrRemoved.Broadcast(CallbackTag, NewCount);
			const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
			if (NewCount == 0) AbilitySystemComponent->RemoveLooseGameplayTag(GameplayTags.Execute);
			else AbilitySystemComponent->AddLooseGameplayTag(GameplayTags.Execute);
		});
		AbilitySystemComponent->OnGameplayEffectAppliedDelegateToTarget.AddLambda([this] (UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
		{
			FGameplayTagContainer GrantedTags;
			SpecApplied.GetAllGrantedTags(GrantedTags);
			if (GrantedTags.HasTag(FBaseGameplayTags::Get().Indicator_Execute))
			{
				if (!AbilitySystemComponent->OnGameplayEffectStackChangeDelegate(ActiveHandle)->IsBound())
				{
					AbilitySystemComponent->OnGameplayEffectStackChangeDelegate(ActiveHandle)->AddLambda([this, SpecApplied] (FActiveGameplayEffectHandle Handle, int32 NewStackCount, int32 PreviousStackCount)
					{
						const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
						OnGameplayTagAddedOrRemoved.Broadcast(GameplayTags.Indicator_Execute, NewStackCount);
					});
				}
			}
		});
		OnHealthChanged.Broadcast(BaseAS->GetHealth());
		OnMaxHealthChanged.Broadcast(BaseAS->GetMaxHealth());
	}
}

void AAICharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	InitializeDefaultAttributes();
	OnASCRegistered.Broadcast(AbilitySystemComponent);
}

void AAICharacter::InitializeDefaultAttributes() const
{
	if(HasAuthority())
	{
		ULeyrAbilitySystemLibrary::InitializeEncounterAttributes(this, EncounterName, Level, AbilitySystemComponent);
	}
}

void AAICharacter::AddAICharacterAbilities() const
{
	if (HasAuthority())
	{
		ULeyrAbilitySystemLibrary::GiveEncounterAbilities(this, AbilitySystemComponent, EncounterName);
	}
}

void AAICharacter::HandleBehaviourState(EBehaviourState NewState)
{
	BehaviourState = NewState;
	
	switch (BehaviourState)
	{
	case EBehaviourState::Patrol:
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		GetCharacterMovement()->MaxFlySpeed = BaseFlySpeed;
		break;
	case EBehaviourState::Search:
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		GetCharacterMovement()->MaxFlySpeed = BaseFlySpeed;
		break;
	case EBehaviourState::Chase:
		GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
		GetCharacterMovement()->MaxFlySpeed = ChasingSpeed;
		break;
	case EBehaviourState::Fall:
		break;
	case EBehaviourState::Dive:
		GetCharacterMovement()->MaxFlySpeed = DivingSpeed;
		break;
	}
}

void AAICharacter::InitializeCharacterInfo()
{
	if(!HasAuthority()) return;
	
	const ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(this));
	if (LeyrGameMode == nullptr || EncounterName == EEncounterName::Default) return;

	const FBehaviourDefaultInfo Info = LeyrGameMode->EncounterInfo->GetEncounterDefaultInfo(EncounterName).BehaviourDefaultInfo;
	BehaviorTree = Info.BehaviorTree;
	BehaviourType = Info.BehaviourType;
	EncounterSize = Info.EncounterSize;
	SineMoveHeight = Info.SineMoveHeight;
	PatrolRadius = Info.PatrolRadius;
	PatrolTickRadius = Info.PatrolTickRadius;
	AttackRange = Info.AttackRange;
	AttackCooldown = Info.AttackCooldown;
	CloseRange = Info.CloseRange;
	ChasingSpeed = Info.ChasingSpeed;
	ChasingHeightOffset = Info.ChasingHeightOffset;
	DivingSpeed = Info.DivingSpeed;
	bCollisionCauseDamage = Info.bCollisionCauseDamage;
	ImpactEffect = Info.ImpactEffect;
	DefeatedSound = Info.DeathSound;
	HitReactSequence = Info.HitReactSequence;
	AttackSequenceInfo = Info.AttackSequenceInfo;
	if(bCollisionCauseDamage)
	{
		bShouldApplyInvincibility = Info.bShouldApplyInvincibility;	
		AbilityPower = Info.AbilityPower;
		DamageEffectClass = Info.DamageEffectClass;
		DamageType = Info.DamageType;
	}
}

void AAICharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && Cast<IAbilitySystemInterface>(OtherActor) && ULeyrAbilitySystemLibrary::IsHostile(this, OtherActor))
	{
		CauseDamage(OtherActor);
	}
}

void AAICharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (BaseAIController && BaseAIController->GetBlackboardComponent())
	{
		BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

/*
 * Combat Interface
 */
void AAICharacter::Die(const FVector& DeathImpulse, bool bExecute)
{
	SetLifeSpan(LifeSpan);
	if (BaseAIController) BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	if(Arena)
	{
		Arena->OnPlayerEntering.Clear();
		Arena->OnPlayerLeaving.Clear();
		Arena->SetTargetActor(nullptr);
	}
	BaseAIController->StopMovement();
	PassiveIndicatorComponent->DestroyComponent();
	HealthBar->DestroyComponent();
	
	if(CombatTarget)
	{
		IPlayerInterface::Execute_SetCameraInterpolation(CombatTarget, Arena, ECameraInterpState::Following);
	}
	
	Super::Die(DeathImpulse, bExecute);
}

void AAICharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
	if(ABaseCharacter* Player = Cast<ABaseCharacter>(CombatTarget))
	{
		if (!Player->OnDeath.IsAlreadyBound(this, &AAICharacter::HandleCombatTargetDefeated))
		{
			Player->OnDeath.AddDynamic(this, &AAICharacter::HandleCombatTargetDefeated);
		}
	}
}

void AAICharacter::HandleCombatTargetDefeated(AActor* Actor)
{
	if (BaseAIController) BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("ShouldEndCombat"), true);
	if (UAICharacterAnimInstance* Instance = Cast<UAICharacterAnimInstance>(AnimationComponent->GetAnimInstance()))
	{
		Instance->bIsCombatTargetDefeated = true;
	}
}

/*
 * AI Interface
 */
FVector AAICharacter::FindRandomLocation_Implementation()
{
	int32 Index = 0;
	while (Index < 10)
	{
		FVector TargetLocation = GetActorLocation() + FVector(FMath::RandRange(-PatrolTickRadius, PatrolTickRadius), 0.f, FMath::RandRange(-PatrolTickRadius, PatrolTickRadius));
		TargetLocation = FVector{
			FMath::Clamp(TargetLocation.X, NavigationBounds.Origin.X - NavigationBounds.BoxExtent.X, NavigationBounds.Origin.X + NavigationBounds.BoxExtent.X),
			0.f,
			FMath::Clamp(TargetLocation.Z, NavigationBounds.Origin.Z - NavigationBounds.BoxExtent.Z, NavigationBounds.Origin.Z + NavigationBounds.BoxExtent.Z) };
		
		if (FMath::Abs((StartLocation - TargetLocation).Size()) > PatrolRadius)
		{
			Index++;
		}
		else
		{
			return TargetLocation;
		}
	}
	return StartLocation;
}

bool AAICharacter::MoveToLocation_Implementation(FVector TargetLocation, float Threshold)
{
	if ((GetActorLocation() - TargetLocation).Size() > Threshold)
	{
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(TargetLocation.X, GetActorLocation().Y, TargetLocation.Z));
		const FRotator WorldDirection = FRotator(LookAtRotation.Pitch, 0.f, 0.f);
		AddMovementInput(LookAtRotation.Vector(), 1.f, true);
		if(FVector::DotProduct(LookAtRotation.Vector(), GetActorForwardVector()) < 0.f) ChangeDirections();
		return false;
	}
	return true;
}

bool AAICharacter::IsTargetWithinEnteringBounds(const FVector& TargetLocation) const
{
	const float Left = EnteringBounds.Origin.X - EnteringBounds.BoxExtent.X;
	const float Right = EnteringBounds.Origin.X + EnteringBounds.BoxExtent.X;
	const float Top = EnteringBounds.Origin.Z + EnteringBounds.BoxExtent.Z;
	const float Bottom = EnteringBounds.Origin.Z - EnteringBounds.BoxExtent.Z;	
	return FMath::IsWithin(TargetLocation.X, Left, Right) && FMath::IsWithin(TargetLocation.Z, Bottom, Top);
}

bool AAICharacter::ChaseTarget_Implementation(AActor* TargetToChase)
{		
	switch (ChasingState) {
	case EChasingState::Chasing:
		if (GetDistanceTo(TargetToChase) > AttackRange)
		{
			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(TargetToChase->GetActorLocation().X, GetActorLocation().Y, TargetToChase->GetActorLocation().Z + ChasingHeightOffset));
			AddMovementInput(LookAtRotation.Vector(), 1.f, true);
			if(FVector::DotProduct(LookAtRotation.Vector(), GetActorForwardVector()) < 0.f) ChangeDirections();
			return false;
		}
		if (GetDistanceTo(TargetToChase) < CloseRange)
		{
			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(TargetToChase->GetActorLocation().X, GetActorLocation().Y, TargetToChase->GetActorLocation().Z + ChasingHeightOffset));
			AddMovementInput(LookAtRotation.Vector(), -.33f, true);
			if(FVector::DotProduct(LookAtRotation.Vector(), GetActorForwardVector()) < 0.f) ChangeDirections();
			return false;
		}
		break;
	case EChasingState::HitReacting:
		break;
	case EChasingState::Defeat:
		break;
	}
	return true;
}

void AAICharacter::SineMove_Implementation()
{
	const float SineValue = FMath::Sin(GetActorLocation().X / 100.f);
	AddMovementInput(GetActorForwardVector() + FVector::UpVector * SineValue, 1.f, true);
}

void AAICharacter::FaceTarget_Implementation()
{
	if (CombatTarget)
	{
		if (CombatTarget->GetActorLocation().X < GetActorLocation().X && FMath::Sign(CombatTarget->GetActorForwardVector().X) < 0 ||
			CombatTarget->GetActorLocation().X > GetActorLocation().X && FMath::Sign(CombatTarget->GetActorForwardVector().X) > 0)
		{
			ChangeDirections();
		}
	}
}

bool AAICharacter::FollowSpline_Implementation(const int32 SplineIndex)
{
	if(SplineComponentActor == nullptr) return false;
	if(SplineComponent == nullptr) SplineComponent = SplineComponentActor->GetSplineComponent();
	const FVector Destination = SplineComponent->GetLocationAtSplinePoint(SplineIndex, ESplineCoordinateSpace::World);
	const FVector Direction = (Destination - GetActorLocation()).GetSafeNormal();
	AddMovementInput(Direction, 1.f);

	return FVector::Distance(GetActorLocation(), Destination) < 20.f;
}

FVector AAICharacter::GetNextLocation_Implementation(const int32 SplineIndex)
{
	if(SplineComponentActor == nullptr) return GetActorLocation();
	if(SplineComponent == nullptr) SplineComponent = SplineComponentActor->GetSplineComponent();
	return  SplineComponent->GetLocationAtSplinePoint(SplineIndex, ESplineCoordinateSpace::World);
}

void AAICharacter::StartSplineMovement_Implementation()
{
	// FActorSpawnParameters SpawnParameters;
	// ASplineComponentActor* SplineComponentActor = GetWorld()->SpawnActor<ASplineComponentActor>(GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
	if(CombatTarget == nullptr) return;
	
	SplinePoints.Empty();
	SplinePoints.Add(GetActorLocation());
	SplinePoints.Add(CombatTarget->GetActorLocation());
	SplinePoints.Add(CombatTarget->GetActorLocation() + GetActorForwardVector() * 300.f);
	SplinePoints.Add(FVector(CombatTarget->GetActorLocation().X + GetActorForwardVector().X * 900.f, 0.f, GetActorLocation().Z));
	// SplineComponentActor->GetSplineComponent()->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::World);
}

bool AAICharacter::FollowSplinePoints_Implementation(int32 SplineIndex)
{
	if(SplinePoints.IsEmpty()) return false;
	const FVector Destination = SplinePoints.IsValidIndex(SplineIndex) ? SplinePoints[SplineIndex] : FVector::ZeroVector;
	const FVector Direction = (Destination - GetActorLocation()).GetSafeNormal();
	AddMovementInput(Direction, 1.f);

	return FVector::Distance(GetActorLocation(), Destination) < 40.f;
}

void AAICharacter::SetNewMovementSpeed_Implementation(EMovementMode InMovementMode, float NewSpeed)
{
	switch (InMovementMode) {
	case MOVE_None:
		break;
	case MOVE_Walking:
		GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
		break;
	case MOVE_NavWalking:
		break;
	case MOVE_Falling:
		break;
	case MOVE_Swimming:
	case MOVE_Flying:
		GetCharacterMovement()->MaxFlySpeed = NewSpeed;
		break;
	case MOVE_Custom:
		break;
	case MOVE_MAX:
		break;
	}
}

void AAICharacter::SetShouldAttack_Implementation(bool InShouldAttack)
{
	ShouldAttack(InShouldAttack);
}

void AAICharacter::ShouldAttack(bool InShouldAttack)
{
	bShouldAttack = InShouldAttack;
	if(IsValid(BaseAIController))
	{
		if (UBlackboardComponent* BlackboardComponent = BaseAIController->GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsBool(FName("ShouldAttack"), InShouldAttack);
		}
	}
}

void AAICharacter::CauseDamage(AActor* TargetActor)
{
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if(TargetASC && TargetASC->HasMatchingGameplayTag(FBaseGameplayTags::Get().Invincibility)) return;
	
	const FGameplayEffectSpecHandle DamageSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, Level, SourceASC->MakeEffectContext());	
	const float ScaledDamage = AbilityPower.GetRandomFloatFromScalableRange(Level);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
	SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);

	if (bShouldApplyInvincibility) ULeyrAbilitySystemLibrary::ApplyInvincibilityToTarget(TargetASC, 1.25f);
}

void AAICharacter::ResetShouldAttack()
{
	if(ResetAttackTimer.IsValid()) ResetAttackTimer.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(ResetAttackTimer, FTimerDelegate::CreateLambda([this] ()
	{
		ShouldAttack(true);
	}), AttackCooldown, false);
}

void AAICharacter::HandlePlayerOverlappingArena(AActor* Player, bool bIsEntering)
{
	if(bIsEntering)
	{
		if (IsValid(BaseAIController)) BaseAIController->GetBlackboardComponent()->SetValueAsObject(FName("TargetToFollow"), Player);
		CombatTarget = Player;
	}
	else
	{
		if (IsValid(BaseAIController)) BaseAIController->GetBlackboardComponent()->SetValueAsObject(FName("TargetToFollow"), nullptr);
		CombatTarget = nullptr;
	}
}

FName AAICharacter::GetNextBehaviourPattern_Implementation(const FName PatternName)
{
	FGameplayTag PatternTag = UGameplayTagsManager::Get().RequestGameplayTag(PatternName, false);
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	if(!PatternTag.MatchesTag(GameplayTags.AI_Pattern)) PatternTag = GameplayTags.AI_Pattern_Random;
	
	if(PatternTag.MatchesTagExact(GameplayTags.AI_Pattern_Random))
	{
		return BehaviourPatterns[FMath::RandRange(0, BehaviourPatterns.Num() - 1)].GetTagName();
	}
	int32 Index = BehaviourPatterns.IndexOfByKey(PatternTag);
	
	if(Index == INDEX_NONE) return GameplayTags.AI_Pattern_None.GetTagName();
	
	Index = Index + 1 >= BehaviourPatterns.Num() ? 0 : Index + 1;
	return BehaviourPatterns[Index].GetTagName();
}

FBoundLocations AAICharacter::CalculateBoundsAtActorZ() const
{
	const FBoundLocations Bounds = Arena->GetArenaBounds();
	const FVector Left = FVector{ Bounds.Left.X, 0.f, GetActorLocation().Z };
	const FVector Right = FVector{ Bounds.Right.X, 0.f, GetActorLocation().Z };
	return FBoundLocations{ Left, Right };
}

FBoundLocations AAICharacter::GetArenaBounds_Implementation()
{
	if(Arena)
	{
		return CalculateBoundsAtActorZ();
	}
	return FBoundLocations();
}

bool AAICharacter::IsTargetWithinEnteringBounds_Implementation(const FVector& Location)
{
	return IsTargetWithinEnteringBounds(Location);
}

bool AAICharacter::IsWithinBounds_Implementation(const FVector& Location)
{
	if(Arena == nullptr) return false;
	
	const FBoundLocations BoundLocations = CalculateBoundsAtActorZ();
	const FVector Extent = FVector{ GetCapsuleComponent()->GetScaledCapsuleRadius() * 2.f, 0.f, 200.f };
	
	if (GetActorForwardVector().X < 0.f)
	{
		return FVector::Distance(Location, BoundLocations.Right) > Extent.X * 2.f;
	}
	if (GetActorForwardVector().X > 0.f)
	{
		return FVector::Distance(Location, BoundLocations.Left) > Extent.X * 2.f;
	}
	return false;
}

FBoxSphereBounds AAICharacter::GetEnteringBounds_Implementation()
{
	return EnteringBounds;
}
