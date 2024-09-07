// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/Breakable.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"

ABreakable::ABreakable()
{
	Tags.Add(FName("HitInteraction"));
	GetRenderComponent()->SetLooping(false);
	HitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
	HitBox->SetupAttachment(GetRootComponent());
}

void ABreakable::Interact_Implementation()
{
	Health--;
	Index++;
	UPaperFlipbookComponent* Flipbook = GetRenderComponent();
	Flipbook->SetPlaybackPositionInFrames(Index, true);
	if(Health <= 0)
	{
		Flipbook->Play();
		SetLifeSpan(.5f);
	}
}

void ABreakable::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	GetRenderComponent()->SetPlaybackPositionInFrames(0, true);
}
