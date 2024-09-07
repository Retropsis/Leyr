// @ Retropsis 2024-2025.

#include "World/Level/Lever.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ALever::ALever()
{
	bReplicates = true;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	OverlapBox->SetupAttachment(GetRootComponent());
}

void ALever::Interact_Implementation()
{
	switch (LeverType) {
	case ELeverType::Switch:
		LeverState = LeverState == ELeverState::Off ?  ELeverState::On : ELeverState::Off;
		break;
	case ELeverType::Timer:
		LeverState = LeverState == ELeverState::Off ?  ELeverState::On : ELeverState::On;
		break;
	case ELeverType::SingleUse:
		LeverState = ELeverState::On;
		break;
	case ELeverType::Weight:
		break;
	}
	HandleLeverVisualState(LeverState);
	OnLeverStateChanged.Broadcast(LeverState);
}

void ALever::HandleLeverVisualState(ELeverState NewState) const
{
	switch (NewState) {
	case ELeverState::None:
		break;
	case ELeverState::On:
		GetRenderComponent()->SetPlaybackPositionInFrames(2, true);
		if(SwitchOnSound) UGameplayStatics::PlaySoundAtLocation(this, SwitchOnSound, GetActorLocation());
		break;
	case ELeverState::Off:
		GetRenderComponent()->SetPlaybackPositionInFrames(1, true);
		if(SwitchOffSound) UGameplayStatics::PlaySoundAtLocation(this, SwitchOffSound, GetActorLocation());
		break;
	case ELeverState::Timer:
		if(TimerTickSound) UGameplayStatics::PlaySoundAtLocation(this, TimerTickSound, GetActorLocation());
		break;
	}
}

void ALever::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority() && LeverType == ELeverType::Weight)
	{
		OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ALever::OnBeginOverlap);
		OverlapBox->OnComponentEndOverlap.AddDynamic(this, &ALever::OnEndOverlap);
	}
}

void ALever::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LeverState = ELeverState::On;
}

void ALever::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	LeverState = ELeverState::Off;
}
