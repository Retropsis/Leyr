// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/Breakable.h"
#include "PaperFlipbookComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

ABreakable::ABreakable()
{	
	HitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
	SetRootComponent(HitBox);
	
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("FlipbookComponent");
	FlipbookComponent->SetupAttachment(GetRootComponent());
	FlipbookComponent->SetLooping(false);
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InitialLifeSpan = 0.f;
	LifeSpan = .2f;
}

void ABreakable::BeginPlay()
{
	Super::BeginPlay();
	// FlipbookComponent->SetPlaybackPositionInFrames(0, true);
	FlipbookComponent->SetPlayRate(1.f);
	FlipbookComponent->Stop();

	if (const UBaseAttributeSet* BaseAS = Cast<UBaseAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAS->GetHealthAttribute()).AddLambda(
			[this, BaseAS] (const FOnAttributeChangeData& Data)
			{
				// OnHealthChanged.Broadcast(Data.NewValue);
				const float Ratio = UKismetMathLibrary::SafeDivide(Data.NewValue, BaseAS->GetMaxHealth());
				const float MappedRatio = UKismetMathLibrary::MapRangeClamped(Ratio, 1.f, 0.f, 1.f, MaxHealthyFrame);
				GEngine->AddOnScreenDebugMessage(3247, 5.f, FColor::Cyan, FString::Printf(TEXT("Ratio: %f - Mapped: %f"), Ratio, MappedRatio));
				FlipbookComponent->SetPlaybackPositionInFrames(FMath::RoundToInt32(MappedRatio), true);
			}
		);
		// AbilitySystemComponent->RegisterGameplayTagEvent(FBaseGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAICharacter::HitReactTagChanged);
		// OnHealthChanged.Broadcast(BaseAS->GetHealth());
	}
	GEngine->AddOnScreenDebugMessage(654987, 5.f, FColor::Cyan, FString::Printf(TEXT("BeginPlay")));
}

void ABreakable::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	FlipbookComponent->SetPlaybackPositionInFrames(0, true);
}

void ABreakable::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);
	MulticastHandleDeath(DeathImpulse);
}

void ABreakable::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	Super::MulticastHandleDeath_Implementation(DeathImpulse);
	FlipbookComponent->Play();
}
