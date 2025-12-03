// @ Retropsis 2024-2025.

#include "World/Level/Event/Lever.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ALever::ALever()
{
	bReplicates = true;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	OverlapBox->SetupAttachment(GetRootComponent());
}

void ALever::LoadActor_Implementation()
{
	if (EventState == EEventState::On)
	{
		HandleLeverVisualState(EventState);
		OnEventStateChanged.Broadcast(EventState);
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
	if(EventState == EEventState::On) return;
	
	EventState = EEventState::On;
	HandleLeverVisualState(EventState);
	OnEventStateChanged.Broadcast(EventState);
	if(const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(OnTimer, this, &ALever::HandleOnTimerEnd, OnTime);
}

void ALever::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// LeverState = ELeverState::Off;
}

void ALever::Interact_Implementation(AActor* InteractingActor)
{
	if(LeverType != ELeverType::Switch && EventState == EEventState::On) return;
	
	switch (LeverType) {
	case ELeverType::Switch:
		EventState = EventState == EEventState::Off ?  EEventState::On : EEventState::Off;
		break;
	case ELeverType::Timer:
		EventState = EEventState::On;
		if(const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(OnTimer, this, &ALever::HandleOnTimerEnd, OnTime);
		break;
	case ELeverType::SingleUse:
		EventState = EEventState::On;
		break;
	case ELeverType::Weight:
		break;
	}
	HandleLeverVisualState(EventState);
	OnEventStateChanged.Broadcast(EventState);
}

void ALever::HandleLeverVisualState(EEventState NewState)
{
	switch (NewState) {
	case EEventState::None:
		break;
	case EEventState::On:
		GetRenderComponent()->SetPlaybackPositionInFrames(2, true);
		if(SwitchOnSound) UGameplayStatics::PlaySoundAtLocation(this, SwitchOnSound, GetActorLocation());
		break;
	case EEventState::Off:
		GetRenderComponent()->SetPlaybackPositionInFrames(1, true);
		if(SwitchOffSound) UGameplayStatics::PlaySoundAtLocation(this, SwitchOffSound, GetActorLocation());
		break;
	case EEventState::Timer:
		GetRenderComponent()->SetPlaybackPositionInFrames(2, true);
		if(TimerTickSound) UGameplayStatics::PlaySoundAtLocation(this, TimerTickSound, GetActorLocation());
		break;
	}
}

void ALever::HandleOnTimerEnd()
{
	EventState = EEventState::Off;
	HandleLeverVisualState(EventState);
	OnEventStateChanged.Broadcast(EventState);
}
