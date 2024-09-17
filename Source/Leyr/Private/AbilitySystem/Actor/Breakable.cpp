// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/Breakable.h"
#include "PaperFlipbookComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

ABreakable::ABreakable()
{	
	HitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
	HitBox->SetBoxExtent(FVector(32.f, 100.f, 32.f));
	HitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	HitBox->SetCollisionResponseToChannel(ECC_Enemy, ECR_Block);
	HitBox->SetCollisionResponseToChannel(ECC_Player, ECR_Block);
	HitBox->SetCollisionResponseToChannel(ECC_Projectile, ECR_Block);
	SetRootComponent(HitBox);
	
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("FlipbookComponent");
	FlipbookComponent->SetupAttachment(GetRootComponent());
	FlipbookComponent->SetLooping(false);
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	TopFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("TopFlipbookComponent");
	TopFlipbookComponent->SetupAttachment(FlipbookComponent);
	TopFlipbookComponent->SetLooping(false);
	TopFlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	InitialLifeSpan = 0.f;
	LifeSpan = .2f;
}

void ABreakable::BeginPlay()
{
	Super::BeginPlay();
	FlipbookComponent->SetPlayRate(1.f);
	FlipbookComponent->Stop();
	TopFlipbookComponent->SetPlayRate(1.f);
	TopFlipbookComponent->Stop();

	if (const UBaseAttributeSet* BaseAS = Cast<UBaseAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAS->GetHealthAttribute()).AddLambda(
			[this, BaseAS] (const FOnAttributeChangeData& Data)
			{
				// OnHealthChanged.Broadcast(Data.NewValue);
				// const float Ratio = UKismetMathLibrary::SafeDivide(Data.NewValue, BaseAS->GetMaxHealth());
				// const float MappedRatio = UKismetMathLibrary::MapRangeClamped(Ratio, 1.f, 0.f, 0.f, MaxHealthyFrame + 1);
				// GEngine->AddOnScreenDebugMessage(3247, 5.f, FColor::Cyan, FString::Printf(TEXT("Ratio: %f - Mapped: %d"), Ratio, FMath::RoundToInt32(MappedRatio)));
				// FlipbookComponent->SetPlaybackPositionInFrames(FMath::RoundToInt32(MappedRatio) + 1, true);

				FlipbookComponent->SetPlaybackPositionInFrames(FlipbookComponent->GetPlaybackPositionInFrames() + 1, true);
				TopFlipbookComponent->SetPlaybackPositionInFrames(TopFlipbookComponent->GetPlaybackPositionInFrames() + 1, true);
				GEngine->AddOnScreenDebugMessage(3247, 5.f, FColor::Cyan, FString::Printf(TEXT("Frame Number: %d"), FlipbookComponent->GetPlaybackPositionInFrames()));
			}
		);
		// AbilitySystemComponent->RegisterGameplayTagEvent(FBaseGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAICharacter::HitReactTagChanged);
		// OnHealthChanged.Broadcast(BaseAS->GetHealth());
	}
}

void ABreakable::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	FlipbookComponent->SetTranslucentSortPriority(GetActorLocation().Z);
	TopFlipbookComponent->SetTranslucentSortPriority(GetActorLocation().Z - 1000);
	
	FlipbookComponent->SetPlaybackPositionInFrames(1, true);
	TopFlipbookComponent->SetPlaybackPositionInFrames(1, true);

	const FVector Start = GetActorLocation() + FVector(0.f, 0.f, FlipbookComponent->Bounds.BoxExtent.Z);
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingle(this, Start, Start + FVector(0.f, 0.f, 10.f), TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, true);
	// if(Hit.bBlockingHit && Hit.GetActor())
	// {
	// 	if (ABreakable* AboveBreakable = Cast<ABreakable>(Hit.GetActor()))
	// 	{
	// 		AboveBreakable->OnBreakableDestroyed.AddLambda([this] () { TopFlipbookComponent->SetVisibility(true); });
	// 	}
	// }
	TopFlipbookComponent->SetVisibility(!Hit.bBlockingHit);	
}

void ABreakable::DestroyActor_Implementation()
{
	Super::DestroyActor_Implementation();
	MulticastHandleDestruction();
}

void ABreakable::MulticastHandleDestruction_Implementation()
{
	Super::MulticastHandleDestruction_Implementation();
	FlipbookComponent->Play();
	TopFlipbookComponent->Play();
	TopFlipbookComponent->SetVisibility(true);
	// OnBreakableDestroyed.Broadcast();
}
