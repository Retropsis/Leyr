// @ Retropsis 2024-2025.

#include "AI/AICharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AI/BaseAIController.h"
#include "AI/SplineComponentActor.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/BaseGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
	BaseAIController = Cast<ABaseAIController>(NewController);
	BaseAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	BaseAIController->RunBehaviorTree(BehaviorTree);
	BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
	BaseAIController->SetPawn(this);

	StartLocation = GetActorLocation();

	if(bCollisionCauseDamage)
	{
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnBeginOverlap);
	}
	SineMoveHeight = GetActorLocation().Z;
	
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
	}
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	GetCharacterMovement()->GravityScale = BaseGravityScale;
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		ULeyrAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}
	
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
		AbilitySystemComponent->RegisterGameplayTagEvent(FBaseGameplayTags::Get().Indicator_Execute, EGameplayTagEventType::NewOrRemoved).AddLambda(
		[this] (const FGameplayTag CallbackTag, int32 NewCount)
		{
			OnGameplayTagAddedOrRemoved.Broadcast(CallbackTag, NewCount);
		});
		
		OnHealthChanged.Broadcast(BaseAS->GetHealth());
		OnMaxHealthChanged.Broadcast(BaseAS->GetMaxHealth());
	}
}

void AAICharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	if (HasAuthority())
	{
		InitializeDefaultAttributes();		
	}
	OnASCRegistered.Broadcast(AbilitySystemComponent);
}

void AAICharacter::InitializeDefaultAttributes() const
{
	if(CharacterClass == ECharacterClass::Default && !Name.IsNone())
	{
		ULeyrAbilitySystemLibrary::InitializeEncounterAttributes(this, Name, Level, AbilitySystemComponent);
	}
	else
	{
		ULeyrAbilitySystemLibrary::InitializeCharacterClassAttributes(this, CharacterClass, Level, AbilitySystemComponent);
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
void AAICharacter::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpan);
	if (BaseAIController) BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	Super::Die(DeathImpulse);
}

/*
 * AI Interface
 */
FVector AAICharacter::FindRandomLocation_Implementation()
{
	// UKismetSystemLibrary::DrawDebugCircle(this, StartLocation, PatrolRadius, 12, FLinearColor::White, 5.f, 0, FVector(1, 0, 0));
	// UKismetSystemLibrary::DrawDebugCircle(this, GetActorLocation(), PatrolTickRadius, 12, FLinearColor::Green, 5.f, 0, FVector(1, 0, 0));
	int32 Index = 0;
	while (Index < 10)
	{
		FVector TargetLocation = GetActorLocation() + FVector(FMath::RandRange(-PatrolTickRadius, PatrolTickRadius), 0.f, FMath::RandRange(-PatrolTickRadius, PatrolTickRadius));
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
		// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + LookAtRotation.Vector() * 100.f, 5.f, FLinearColor::Red, 1.f);
		const FRotator WorldDirection = FRotator(LookAtRotation.Pitch, 0.f, 0.f);
		// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + WorldDirection.Vector() * 100.f, 5.f, FLinearColor::Green, 1.f);
		AddMovementInput(LookAtRotation.Vector(), 1.f, true);
		if(FVector::DotProduct(LookAtRotation.Vector(), GetActorForwardVector()) < 0.f) ChangeDirections();
		return false;
	}
	return true;
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
