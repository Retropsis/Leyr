// @ Retropsis 2024-2025.

#include "AI/AICharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "NiagaraSystem.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AbilitySystem/Effect/StatusEffectNiagaraComponent.h"
#include "AI/AICharacterAnimInstance.h"
#include "AI/BaseAIController.h"
#include "AI/SplineComponentActor.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "Data/AttributeData.h"
#include "Data/BehaviourData.h"
#include "Data/EncounterData.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Game/BaseGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "World/Data/CameraData.h"
#include "World/Level/Moving/WaterGroup.h"
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
	HealthBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HealthBar->SetCollisionResponseToAllChannels(ECR_Ignore);
	HealthBar->SetGenerateOverlapEvents(false);
	
	PassiveIndicatorComponent = CreateDefaultSubobject<UWidgetComponent>("PassiveIndicator");
	PassiveIndicatorComponent->SetupAttachment(GetRootComponent());
	PassiveIndicatorComponent->SetWidgetSpace(EWidgetSpace::Screen);
	PassiveIndicatorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PassiveIndicatorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	PassiveIndicatorComponent->SetGenerateOverlapEvents(false);
}

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
	AbilitySystemComponent->RegisterGameplayTagEvent(FBaseGameplayTags::Get().StatusEffect_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAICharacter::StunTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(FBaseGameplayTags::Get().StatusEffect_Burn, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAICharacter::BurnTagChanged);

	StartLocation = GetActorLocation();
	
	BaseAIController = Cast<ABaseAIController>(NewController);
	BaseAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	BaseAIController->RunBehaviorTree(BehaviorTree);
	BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
	BaseAIController->GetBlackboardComponent()->SetValueAsVector(FName("StartLocation"), StartLocation);
	BaseAIController->GetBlackboardComponent()->SetValueAsObject(FName("OwningSummoner"), OwningSummoner);
	if (IsValid(SplineComponent))
	{
		// SplineComponent = SplineComponentActor->SplineComponent;
		BaseAIController->GetBlackboardComponent()->SetValueAsObject(FName("SplineComponent"), SplineComponent);
	}
	BaseAIController->SetPawn(this);

	if (Arena)
	{
		Arena->OnPlayerEntering.AddLambda([this](AActor* Player) { HandlePlayerOverlappingArena(Player, true); });
		Arena->OnPlayerLeaving.AddLambda([this](AActor* Player) { HandlePlayerOverlappingArena(Player, false); });
		Arena->SetTargetActor(this);
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
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
		break;
	case EBehaviourType::Aquatic:
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
		GetCharacterMovement()->BrakingDecelerationFlying = 200.f;
		break;
	}
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	checkf(EncounterData, TEXT("CharacterInfo is missing on [%s]"), *GetName());
	checkf(EncounterData->AbilitySet, TEXT("AbilitySet is missing on [%s]"), *EncounterData->GetName());
	
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	GetCharacterMovement()->MaxFlySpeed = BaseFlySpeed;
	GetCharacterMovement()->GravityScale = BaseGravityScale;
	InitAbilityActorInfo();
	AddCharacterAbilities();
	PlaySpawningSequence();
	SetupCollisionDamage();

	// if(bCollisionCauseDamage)
	// {
	// 	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnBeginOverlap);
	// 	SetupCollisionDamage();
	// }
	// GEngine->AddOnScreenDebugMessage(-1, 300.f, FColor::Magenta, FString::Printf(TEXT("BeginPlay bCollisionCauseDamage %s"), *GetName()));
	
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
		AbilitySystemComponent->RegisterGameplayTagEvent(FBaseGameplayTags::Get().StatusEffect_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAICharacter::HitReactTagChanged);
		AbilitySystemComponent->RegisterGameplayTagEvent(FBaseGameplayTags::Get().StatusEffect_Weakened, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAICharacter::WeakenedTagChanged);
		AbilitySystemComponent->RegisterGameplayTagEvent(FBaseGameplayTags::Get().VisualEffect_HitReactFlash, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAICharacter::HitReactFlashTagChanged);
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

void AAICharacter::InitializeCharacterInfo()
{
	if(!HasAuthority()) return;

	checkf(EncounterData, TEXT("Please add EncounterData to %s"), *GetName());
	checkf(EncounterData->BehaviourData, TEXT("Please add DefaultBehaviourData to %s"), *EncounterData->GetName());
	
	bShouldDespawn = EncounterData->bShouldDespawn;
	EncounterSize = EncounterData->EncounterSize;
	CharacterClass = EncounterData->CharacterClass;
	WeaponSocketName = EncounterData->WeaponSocketTag.GetTagName();

	LootData = EncounterData->LootData;
	AttackSequenceInfo = EncounterData->AttackSequenceInfo;
	bSimulatePhysicsOnDestroyed = EncounterData->bSimulatePhysicsOnDestroyed;
	
	BehaviorTree = EncounterData->BehaviourData->BehaviorTree;
	BehaviourType = EncounterData->BehaviourData->BehaviourType;
	MovementType = EncounterData->BehaviourData->MovementType; 
	SineMoveHeight = EncounterData->BehaviourData->SineMoveHeight;
	SineMoveSpeed = EncounterData->BehaviourData->SineMoveSpeed;
	PatrolRadius = EncounterData->BehaviourData->PatrolRadius;
	PatrolTickRadius = EncounterData->BehaviourData->PatrolTickRadius;
	AttackRange = EncounterData->BehaviourData->AttackRange;
	AttackCooldown = EncounterData->BehaviourData->AttackCooldown;
	CloseRange = EncounterData->BehaviourData->CloseRange;
	ChasingSpeed = EncounterData->BehaviourData->ChasingSpeed;
	ChasingHeightOffset = EncounterData->BehaviourData->ChasingHeightOffset;
	DivingSpeed = EncounterData->BehaviourData->DivingSpeed;
	BaseWalkSpeed = EncounterData->BehaviourData->BaseWalkSpeed;
	BaseFlySpeed = EncounterData->BehaviourData->BaseFlySpeed;
	
	bCollisionCauseDamage = EncounterData->BehaviourData->bCollisionCauseDamage;
	if(bCollisionCauseDamage)
	{
		bShouldApplyInvincibility = EncounterData->BehaviourData->bShouldApplyInvincibility;	
		AbilityPower = EncounterData->BehaviourData->AbilityPower.GetValueAtLevel(Level);
		DamageEffectClass = EncounterData->BehaviourData->DamageEffectClass;
		DamageType = EncounterData->BehaviourData->DamageType;

		// GEngine->AddOnScreenDebugMessage(-1, 300.f, FColor::Magenta, FString::Printf(TEXT("PossessedBy InitializeCharacterInfo %s"), *GetName()));
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnBeginOverlap);
		// SetupCollisionDamage();
	}

	// Loading Hit React Sequences Effect
	for (TTuple<FGameplayTag, TSoftObjectPtr<UPaperZDAnimSequence>> HitReactSequence : EncounterData->HitReactSequences)
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

	// Loading Wound Impact Effects
	for (TTuple<FGameplayTag, TSoftObjectPtr<UNiagaraSystem>> WoundImpactEffect : EncounterData->WoundImpactEffects)
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

	// Loading Destroyed Effect
	UAssetManager::GetStreamableManager().RequestAsyncLoad(EncounterData->DestroyedEffect.ToSoftObjectPath(), [this] () {
		UNiagaraSystem* LoadedAsset = EncounterData->DestroyedEffect.Get();
		if (IsValid(LoadedAsset))
		{
			DestroyedEffectLoaded = LoadedAsset;
		}
	}, FStreamableManager::DefaultAsyncLoadPriority);
	
	// Loading Destroyed Sound
	UAssetManager::GetStreamableManager().RequestAsyncLoad(EncounterData->DeathSound.ToSoftObjectPath(), [this] () {
		USoundBase* LoadedAsset = EncounterData->DeathSound.Get();
		if (IsValid(LoadedAsset))
		{
			DefeatedSoundLoaded = LoadedAsset;
		}
	}, FStreamableManager::DefaultAsyncLoadPriority);
}

void AAICharacter::AddCharacterAbilities()
{	
	EncounterData->AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &GrantedHandles, FGameplayTag(), Level, AbilitySystemComponent);
}

void AAICharacter::InitializeDefaultAttributes() const
{
	EncounterData->AttributeData->GiveToAbilitySystem(AbilitySystemComponent, Level);
}

/*
 * Tag Changed Callbacks
 */
void AAICharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	GetCharacterMovement()->MaxFlySpeed = bHitReacting ? 0.f : BaseFlySpeed;
	if (BaseAIController && BaseAIController->GetBlackboardComponent())
	{
		BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void AAICharacter::WeakenedTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	HitReactTagChanged(CallbackTag, NewCount);
}

void AAICharacter::HasFinishedSpawning()
{
	Super::HasFinishedSpawning();
	
	if (BaseAIController && BaseAIController->GetBlackboardComponent())
	{
		BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("HasFinishedSpawning"), true);
	}
}

void AAICharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);
	if (BaseAIController && BaseAIController->GetBlackboardComponent())
	{
		BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), bIsStunned);
	}
}

void AAICharacter::OnRep_Stunned() { Super::OnRep_Stunned(); }
void AAICharacter::OnRep_Burned() { Super::OnRep_Burned(); }

void AAICharacter::InitializeNavigationBounds()
{
	const FVector Start = GetActorLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	TArray<AActor*> OutActors;
	
	if (BehaviourType == EBehaviourType::Aquatic)
	{
		UKismetSystemLibrary::CapsuleOverlapActors(this, Start, GetCapsuleComponent()->GetScaledCapsuleRadius(), GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
		ObjectTypes, AWaterGroup::StaticClass(), ActorsToIgnore, OutActors);
		if (OutActors.Num() > 0)
		{
			for (AActor* OverlapActor : OutActors)
			{
				if (const AWaterGroup* WaterGroup = Cast<AWaterGroup>(OverlapActor))
				{
					NavigationBounds = WaterGroup->GetNavigationBounds();
				}
			}
		}
	}
	OutActors.Empty();
	UKismetSystemLibrary::CapsuleOverlapActors(this, Start, GetCapsuleComponent()->GetScaledCapsuleRadius(), GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
	ObjectTypes, ACameraBoundary::StaticClass(), ActorsToIgnore, OutActors);
	if (OutActors.Num() > 0)
	{
		for (AActor* OverlapActor : OutActors)
		{
			if (const ACameraBoundary* CameraBoundary = Cast<ACameraBoundary>(OverlapActor))
			{
				EnteringBounds = CameraBoundary->GetEnteringBounds(); 
				if (BehaviourType != EBehaviourType::Aquatic) NavigationBounds = CameraBoundary->GetNavigationBounds();
			}
		}
	}
}

void AAICharacter::HandleBehaviourState(EBehaviourState NewState)
{
	BehaviourState = NewState;
	
	switch (BehaviourState)
	{
	case EBehaviourState::Patrol:
	case EBehaviourState::Search:
	case EBehaviourState::Timeline:
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
	case EBehaviourState::Duck: // TODO: Could be Crouch?
		break;
	}
}

void AAICharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandleBeginOverlap(OtherActor);
}

void AAICharacter::HandleBeginOverlap(AActor* OtherActor)
{
	if(OtherActor && Cast<IAbilitySystemInterface>(OtherActor) && ULeyrAbilitySystemLibrary::IsHostile(this, OtherActor))
	{
		CauseDamage(OtherActor);
	}
}

void AAICharacter::SpawnFromEgg()
{
	if (ActorClassToSpawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActorClassToSpawn is null in [%hs]"), __FUNCTION__);
		return;
	}
	
	FTransform SpawnTransform = GetActorTransform();
	SpawnTransform.SetLocation(FVector{ GetActorLocation().X, 5.f, GetActorLocation().Z });
	AAICharacter* Encounter = GetWorld()->SpawnActorDeferred<AAICharacter>(ActorClassToSpawn, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	// Encounter->SetEncounterLevel(Level);
	// Encounter->SetEncounterData(EncounterData);
	Encounter->SplineComponent = SplineComponent;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// IAIInterface::Execute_SetSpawningBounds(Encounter, SpawningBounds);					
	Encounter->SpawnDefaultController();
	Encounter->FinishSpawning(SpawnTransform);
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
	
	if(CombatTarget && CombatTarget->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_SetCameraInterpolation(CombatTarget, Arena, ECameraInterpState::Following);
	}
	
	// DefeatState = bExecute ? EDefeatState::Executed : EDefeatState::Defeated;
	// MulticastHandleDeath(DeathImpulse, DefeatState); // This called ABaseCharacter::MulticastHandleDeath a second time
	SpawnLoot();
	Super::Die(DeathImpulse, bExecute);
}

void AAICharacter::MulticastHandleDeath_Implementation(const FVector& DeathImpulse, EDefeatState InDefeatState)
{
	Super::MulticastHandleDeath_Implementation(DeathImpulse, InDefeatState);
	FVector TargetDestination = GetActorLocation() + FVector(GetActorForwardVector().X * -75.f, 0, 0);
	Execute_TimelineMovementForTag(this, FBaseGameplayTags::Get().Defeated, TargetDestination);
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

// ReSharper disable once CppMemberFunctionMayBeConst
void AAICharacter::HandleCombatTargetDefeated(AActor* Actor)
{
	if (BaseAIController) BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("ShouldEndCombat"), true);
	if (UAICharacterAnimInstance* Instance = Cast<UAICharacterAnimInstance>(AnimationComponent->GetAnimInstance()))
	{
		Instance->bIsCombatTargetDefeated = true;
	}
}

void AAICharacter::ChangePaperAnimInstance_Implementation(const TSubclassOf<UPaperZDAnimInstance> NewInstance)
{
	if (AnimationComponent && NewInstance)
	{
		AnimationComponent->SetAnimInstanceClass(NewInstance);
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

bool AAICharacter::MoveToLocation_Implementation(FVector TargetLocation, float Threshold, bool bBackward, bool bIgnoreZ)
{
	FVector ActorLocation = GetActorLocation();
	if (bIgnoreZ)
	{
		ActorLocation = FVector{ ActorLocation.X, 0.f, TargetLocation.Z };
		TargetLocation = FVector{ TargetLocation.X, 0.f, TargetLocation.Z };
	}
	if ((ActorLocation - TargetLocation).Size() > Threshold)
	{
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ActorLocation, FVector(TargetLocation.X, ActorLocation.Y, TargetLocation.Z));
		// const FRotator WorldDirection = FRotator(LookAtRotation.Pitch, 0.f, 0.f);
		AddMovementInput(LookAtRotation.Vector(), 1.f, true);
		if(!bBackward && FVector::DotProduct(LookAtRotation.Vector(), GetActorForwardVector()) < 0.f) ChangeDirections();
		return false;
	}
	return true;
}

FVector AAICharacter::FindTargetLocation_Implementation(AActor* TargetActor, float DistanceToKeep)
{
	if (TargetActor == nullptr) return FVector::ZeroVector;
	const FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(TargetActor->GetActorLocation(), GetActorLocation());
	return TargetActor->GetActorLocation() + FVector{ Direction.X * DistanceToKeep, 0.f, 0.f };
}

bool AAICharacter::CheckForObstacle_Implementation(float TraceDistance, bool bBackward)
{
	const float Direction = bBackward ? -1.f : 1.f;
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * TraceDistance * Direction;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(CombatTarget);
	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingle(this, Start, End, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true);
	return Hit.bBlockingHit;
}

bool AAICharacter::CheckForGround_Implementation(float TraceDistance, bool bBackward)
{
	const float Direction = bBackward ? -1.f : 1.f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius() * Direction + FVector::DownVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FVector End = Start + FVector::DownVector * TraceDistance;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(CombatTarget);
	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingle(this, Start, End, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true);
	return Hit.bBlockingHit;
}

bool AAICharacter::RequestJump_Implementation()
{
	if (CanJump())
	{
		Jump();
		return true;
	}
	return false;
}

bool AAICharacter::IsTargetWithinEnteringBounds(const FVector& TargetLocation) const
{
	const float Left = EnteringBounds.Origin.X - EnteringBounds.BoxExtent.X;
	const float Right = EnteringBounds.Origin.X + EnteringBounds.BoxExtent.X;
	const float Top = EnteringBounds.Origin.Z + EnteringBounds.BoxExtent.Z;
	const float Bottom = EnteringBounds.Origin.Z - EnteringBounds.BoxExtent.Z;	
	return FMath::IsWithin(TargetLocation.X, Left, Right) && FMath::IsWithin(TargetLocation.Z, Bottom, Top);
}

bool AAICharacter::IsTargetWithinNavigationBounds(const FVector& TargetLocation) const
{
	const float Left = NavigationBounds.Origin.X - NavigationBounds.BoxExtent.X;
	const float Right = NavigationBounds.Origin.X + NavigationBounds.BoxExtent.X;
	const float Top = NavigationBounds.Origin.Z + NavigationBounds.BoxExtent.Z;
	const float Bottom = NavigationBounds.Origin.Z - NavigationBounds.BoxExtent.Z;	
	return FMath::IsWithin(TargetLocation.X, Left, Right) && FMath::IsWithin(TargetLocation.Z, Bottom, Top);
}

bool AAICharacter::ChaseTarget_Implementation(AActor* TargetToChase)
{
	if (!IsValid(TargetToChase)) return true;

	const float IdealRange = (AttackRange + CloseRange) / 2.f;
	const float Side = FMath::Sign(GetActorLocation().X - TargetToChase->GetActorLocation().X);
	
	if (GetDistanceTo(TargetToChase) > AttackRange)
	{
		const FVector PreferredLocation = TargetToChase->GetActorLocation() + FVector{ Side * FMath::Sqrt(IdealRange * IdealRange - ChasingHeightOffset * ChasingHeightOffset), 0.f, ChasingHeightOffset };
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PreferredLocation);
		
		AddMovementInput(LookAtRotation.Vector(), 1.f, true);
		if(FVector::DotProduct( UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetToChase->GetActorLocation()).Vector(), GetActorForwardVector()) < 0.f) ChangeDirections();
		return false;
	}
	if (GetDistanceTo(TargetToChase) < CloseRange)
	{
		const FVector PreferredLocation = TargetToChase->GetActorLocation() + FVector{ Side * FMath::Sqrt(IdealRange * IdealRange - ChasingHeightOffset * ChasingHeightOffset), 0.f, ChasingHeightOffset };
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PreferredLocation);
		
		AddMovementInput(LookAtRotation.Vector(), .33f, true);
		if(FVector::DotProduct( UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetToChase->GetActorLocation()).Vector(), GetActorForwardVector()) < 0.f) ChangeDirections();
		return false;
	}
	return true;
}

bool AAICharacter::ChaseTargetWithinWater_Implementation(AActor* TargetToChase)
{
	if (!IsValid(TargetToChase)) return true;

	const float IdealRange = (AttackRange + CloseRange) / 2.f;
	const float Side = FMath::Sign(GetActorLocation().X - TargetToChase->GetActorLocation().X);
	
	if (GetDistanceTo(TargetToChase) > AttackRange)
	{
		const FVector PreferredLocation = TargetToChase->GetActorLocation() + FVector{ Side * IdealRange, 0.f, 0.f };
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PreferredLocation);
		if (IsTargetWithinNavigationBounds(PreferredLocation))
		{
			AddMovementInput(LookAtRotation.Vector(), 1.f, true);
			if(FVector::DotProduct(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetToChase->GetActorLocation()).Vector(), GetActorForwardVector()) < 0.f) ChangeDirections();
			return false;
		}
		GetCharacterMovement()->StopMovementImmediately();
		return true;
	}
	if (GetDistanceTo(TargetToChase) < CloseRange)
	{
		const FVector PreferredLocation = TargetToChase->GetActorLocation() + FVector{ Side * IdealRange, 0.f, 0.f };
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PreferredLocation);
		if (IsTargetWithinNavigationBounds(PreferredLocation))
		{
			const float ScaleValue = FMath::Max(1.f - (GetActorLocation() - TargetToChase->GetActorLocation()).Length() / IdealRange, .33f);
			AddMovementInput(LookAtRotation.Vector(), ScaleValue, true);
			if(FVector::DotProduct(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetToChase->GetActorLocation()).Vector(), GetActorForwardVector()) < 0.f) ChangeDirections();
			return false;
		}
		GetCharacterMovement()->StopMovementImmediately();
		return true;
	}
	return true;
}

void AAICharacter::SetupFlyAroundTarget_Implementation(FVector TargetLocation, float Radius)
{
	FlyAroundTargetLocation = TargetLocation;
	FlyAroundRadius = Radius;
	FlyAroundDeltaTimeStart = GetWorld()->GetTimeSeconds();
}

void AAICharacter::FlyAroundTarget_Implementation()
{
	const FVector ActorLocation = GetActorLocation();
	const float DeltaTime = GetWorld()->GetTimeSeconds() - FlyAroundDeltaTimeStart;

	float FactorX = 1.f;
	FactorX *= FMath::Sin(DeltaTime * 2.f) * 200.f;
	
	if (FlyAroundTargetLocation.X < ActorLocation.X)
	{
		if (FlyAroundTargetLocation.Z < ActorLocation.Z)
		{
		
		}
		else
		{
			
		}
	}
	else
	{
		if (FlyAroundTargetLocation.Z < ActorLocation.Z)
		{
			
		}
		else
		{
			
		}
	}
	const FVector Movement{ FMath::Cos(DeltaTime * 1.5f), 0.f, FMath::Sin(DeltaTime * 1.5f) };
	// const float CurrentDistance = FMath::FInterpTo((FlyAroundTargetLocation - ActorLocation).Length(), FlyAroundRadius, DeltaTime, 5.f);

	const FVector NewTargetLocation = FVector{ FlyAroundTargetLocation.X + FactorX + Movement.X * FlyAroundRadius, 0.f, FlyAroundTargetLocation.Z + Movement.Z * FlyAroundRadius };
	SetActorLocation(FMath::VInterpConstantTo(ActorLocation, NewTargetLocation, DeltaTime, 2.f));
}

void AAICharacter::StartTimelineMovement_Implementation()
{
	InitialTimelineLocation = GetActorLocation();
	RandomAmplitudeFromRange();
}

void AAICharacter::FaceTarget_Implementation()
{
	if (CombatTarget)
	{
		if (CombatTarget->GetActorLocation().X < GetActorLocation().X && FMath::Sign(GetActorForwardVector().X) > 0 ||
			CombatTarget->GetActorLocation().X > GetActorLocation().X && FMath::Sign(GetActorForwardVector().X) < 0)
		{
			ChangeDirections();
		}
	}
}

bool AAICharacter::FollowSpline_Implementation(const int32 SplineIndex)
{
	if (!IsValid(SplineComponent)) return false;
	
	const FVector Destination = SplineComponent->GetLocationAtSplinePoint(SplineIndex, ESplineCoordinateSpace::World);
	const FVector Direction = (Destination - GetActorLocation()).GetSafeNormal();
	AddMovementInput(Direction, 1.f);

	return FVector::Distance(GetActorLocation(), Destination) < 20.f;
}

bool AAICharacter::FollowSplinePoints_Implementation(int32 SplineIndex)
{
	if(SplinePoints.IsEmpty()) return false;
	const FVector Destination = SplinePoints.IsValidIndex(SplineIndex) ? SplinePoints[SplineIndex] : FVector::ZeroVector;
	const FVector Direction = (Destination - GetActorLocation()).GetSafeNormal();
	AddMovementInput(Direction, 1.f);

	return FVector::Distance(GetActorLocation(), Destination) < 40.f;
}

FVector AAICharacter::GetNextLocation_Implementation(const int32 SplineIndex)
{
	if (!IsValid(SplineComponent)) return GetActorLocation();
	return  SplineComponent->GetLocationAtSplinePoint(SplineIndex, ESplineCoordinateSpace::World);
}

void AAICharacter::StartSplineMovement_Implementation()
{
	if(CombatTarget == nullptr) return;
	
	SplinePoints.Empty();
	SplinePoints.Add(GetActorLocation());
	SplinePoints.Add(CombatTarget->GetActorLocation());
	SplinePoints.Add(CombatTarget->GetActorLocation() + GetActorForwardVector() * 300.f);
	SplinePoints.Add(FVector(CombatTarget->GetActorLocation().X + GetActorForwardVector().X * 900.f, 0.f, GetActorLocation().Z));
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

void AAICharacter::PlaySpawningSequence() const
{
	if (EncounterData == nullptr) return;
	if (!EncounterData->bHasSpawningSequence) return;
	
	if (EncounterData->SpawningSequence.Get() == nullptr)
	{
		UAssetManager::GetStreamableManager().RequestAsyncLoad(EncounterData->SpawningSequence.ToSoftObjectPath(), [this] () {
			AnimationComponent->GetAnimInstance()->PlayAnimationOverride(EncounterData->SpawningSequence.Get());
		}, FStreamableManager::AsyncLoadHighPriority);
	}
	else
	{
		AnimationComponent->GetAnimInstance()->PlayAnimationOverride(EncounterData->SpawningSequence.Get());
	}
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
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, AbilityPower);
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

void AAICharacter::InitializeSineMove()
{
	InitialTimelineLocation = GetActorLocation();
	EndTimelineLocation = FVector{ InitialTimelineLocation.X + GetActorForwardVector().X * SineMoveSpeed, 0.f, InitialTimelineLocation.Z };
}

void AAICharacter::ShouldEndSineMove(UTimelineComponent* Timeline)
{
	// if (!IsTargetWithinEnteringBounds(GetActorLocation()))
	// {
	// 	FTimerHandle StopTimer;
	// 	GetWorld()->GetTimerManager().SetTimer(StopTimer, FTimerDelegate::CreateLambda([this, Timeline] ()
	// 	{
	// 		if(Timeline) Timeline->Stop();
	// 		Destroy();
	// 	}), .5f, false);
	// }
	if (DefeatState > EDefeatState::None) if(Timeline) Timeline->Stop();
}

void AAICharacter::RandomAmplitudeFromRange(const float Min, const float Max)
{
	const float Sign = FMath::RandBool() ? 1.f : -1.f;
	SineAmplitude = FMath::FRandRange(Sign * Min, Sign * Max);
}

ASplineComponentActor* AAICharacter::FindSplineForTag(const FGameplayTag& Tag)
{	
	if (PatternParams.Contains(Tag))
	{
		if (PatternParams[Tag].SplineComponentActors.Num() > 0)
		{
			return  PatternParams[Tag].SplineComponentActors[FMath::RandRange(0, PatternParams[Tag].SplineComponentActors.Num() - 1)];
		}
	}
	return nullptr;
}

ASplineComponentActor* AAICharacter::FindSplineForTagName_Implementation(const FName& TagName)
{
	return FindSplineForTagName(TagName);
}

UPaperZDAnimInstance* AAICharacter::FindPaperAnimInstanceForTag(const FGameplayTag& Tag)
{
	if (PatternParams.Contains(Tag))
	{
		return PatternParams[Tag].PaperAnimInstance.GetDefaultObject();
	}
	return nullptr;
}

UPaperZDAnimInstance* AAICharacter::FindPaperAnimInstanceForTagName_Implementation(const FName& TagName)
{
	return FindPaperAnimInstanceForTagName(TagName);
}

UAnimInstance* AAICharacter::FindMultiPartAnimInstanceForTag(const FGameplayTag& Tag)
{
	if (PatternParams.Contains(Tag))
	{
		return PatternParams[Tag].MultiPartAnimInstance.GetDefaultObject();
	}
	return nullptr;
}

UAnimInstance* AAICharacter::FindMultiPartAnimInstanceForTagName_Implementation(const FName& TagName)
{
	return FindMultiPartAnimInstanceForTagName(TagName);
}

void AAICharacter::FindAndApplyPatternParamsForPattern_Implementation(const FName& PatternName)
{
	const FGameplayTag PatternTag = FGameplayTag::RequestGameplayTag(PatternName);
	if (PatternTag.IsValid() && PatternParams.Contains(PatternTag))
	{
		const FPatternParams Params = PatternParams[PatternTag];
		switch (BehaviourType) {
		case EBehaviourType::Patrol: 
		case EBehaviourType::Ranger:
			GetCharacterMovement()->MaxWalkSpeed = Params.Speed > 0.f ? Params.Speed : BaseWalkSpeed;
			break;
		case EBehaviourType::Turret:
			break;
		case EBehaviourType::Airborne:
		case EBehaviourType::Aquatic:
			GetCharacterMovement()->MaxFlySpeed = Params.Speed > 0.f ? Params.Speed : BaseFlySpeed;
			break;
		}
		if (Params.SplineComponentActors.Num() > 0)
		{
			SplineComponent = Params.SplineComponentActors[FMath::RandRange(0, Params.SplineComponentActors.Num() - 1)]->GetSplineComponent();
		}
		if (AnimationComponent && Params.PaperAnimInstance)
		{
			AnimationComponent->SetAnimInstanceClass(Params.PaperAnimInstance);
		}
		if (Params.MultiPartAnimInstance)
		{
			SetMultiPartAnimInstance(Params.MultiPartAnimInstance);
		}		
	}
}

UPaperZDAnimSequence* AAICharacter::GetDespawnSequence_Implementation()
{
	if (EncounterData == nullptr) return nullptr;
	
	if (EncounterData->DespawnSequence.Get() == nullptr)
	{
		UAssetManager::GetStreamableManager().RequestAsyncLoad(EncounterData->DespawnSequence.ToSoftObjectPath(), [this] () {
			return EncounterData->DespawnSequence.Get();
		}, FStreamableManager::AsyncLoadHighPriority);
	}
	else
	{
		return EncounterData->DespawnSequence.Get();
	}
	return nullptr;
}

void AAICharacter::Despawn_Implementation()
{
	if (BaseAIController) BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	BaseAIController->StopMovement();
	PassiveIndicatorComponent->DestroyComponent();
	HealthBar->DestroyComponent();
	BurnStatusEffectComponent->DeactivateImmediate();
	StunStatusEffectComponent->DeactivateImmediate();
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Player, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Enemy, ECR_Ignore);
}

void AAICharacter::DespawnFinished_Implementation()
{
	// OnDeath.Broadcast(this);
	Destroy();
}
