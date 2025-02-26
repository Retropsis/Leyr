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

void ALever::LoadActor_Implementation()
{
	if (LeverState == ELeverState::On)
	{
		HandleLeverVisualState(LeverState);
		OnLeverStateChanged.Broadcast(LeverState);
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
	if(LeverState == ELeverState::On) return;
	
	LeverState = ELeverState::On;
	HandleLeverVisualState(LeverState);
	OnLeverStateChanged.Broadcast(LeverState);
	if(UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(OnTimer, this, &ALever::HandleOnTimerEnd, OnTime);
}

void ALever::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// LeverState = ELeverState::Off;
}

void ALever::Interact_Implementation(AActor* InteractingActor)
{
	if(LeverType != ELeverType::Switch && LeverState == ELeverState::On) return;
	
	switch (LeverType) {
	case ELeverType::Switch:
		LeverState = LeverState == ELeverState::Off ?  ELeverState::On : ELeverState::Off;
		break;
	case ELeverType::Timer:
		LeverState = ELeverState::On;
		if(UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(OnTimer, this, &ALever::HandleOnTimerEnd, OnTime);
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

void ALever::HandleLeverVisualState(ELeverState NewState)
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
		GetRenderComponent()->SetPlaybackPositionInFrames(2, true);
		if(TimerTickSound) UGameplayStatics::PlaySoundAtLocation(this, TimerTickSound, GetActorLocation());
		break;
	}
}

void ALever::HandleOnTimerEnd()
{
	LeverState = ELeverState::Off;
	HandleLeverVisualState(LeverState);
	OnLeverStateChanged.Broadcast(LeverState);
}
