// @ Retropsis 2024-2025.

#include "AI/AICharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "AI/BaseAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/BaseGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

AAICharacter::AAICharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// TODO: Only for 3D, 2D should snap
	// bUseControllerRotationPitch = false;
	// bUseControllerRotationRoll = false;
	// bUseControllerRotationYaw = false;
	// GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);
	
	PassiveIndicatorComponent = CreateDefaultSubobject<UWidgetComponent>("PassiveIndicator");
	PassiveIndicatorComponent->SetupAttachment(GetRootComponent());
	PassiveIndicatorComponent->SetWidgetSpace(EWidgetSpace::Screen);
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

	StartLocation = GetActorLocation();
	
	switch (BehaviourType) {
	case EBehaviourType::Patrol:
		break;
	case EBehaviourType::Ranger:
		break;
	case EBehaviourType::Turret:
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
	ULeyrAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
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
	int32 Index = 0;
	while (Index < 10)
	{
		FVector TargetLocation = GetActorLocation() + FVector(FMath::RandRange(-PatrolTickRadius, PatrolTickRadius), 0.f, FMath::RandRange(-PatrolTickRadius, PatrolTickRadius));
		if (FMath::Abs((StartLocation - TargetLocation).Length()) > PatrolRadius)
		{
			Index++;
		}
		else
		{
			return TargetLocation;
		}
	}
	return FVector::ZeroVector;
}

bool AAICharacter::MoveToLocation_Implementation(FVector TargetLocation, float Threshold)
{
	if ((GetActorLocation() - TargetLocation).Length() > Threshold)
	{
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(TargetLocation.X, GetActorLocation().Y, TargetLocation.Z));
		UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + LookAtRotation.Vector() * 100.f, 5.f, FLinearColor::Red, 1.f);
		const FRotator WorldDirection = FRotator(LookAtRotation.Pitch, 0.f, 0.f);
		UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + WorldDirection.Vector() * 100.f, 5.f, FLinearColor::Green, 1.f);
		AddMovementInput(WorldDirection.Vector(), 1.f, true);
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Speed: %f"), GetCharacterMovement()->MaxFlySpeed));
		// SetActorRotation() // Set it to 0 or 180
		return true;
	}
	return false;
}
