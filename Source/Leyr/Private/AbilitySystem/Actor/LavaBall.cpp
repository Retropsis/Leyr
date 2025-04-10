// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/LavaBall.h"

ALavaBall::ALavaBall()
{
	PrimaryActorTick.bCanEverTick = false;

	ApexComponent = CreateDefaultSubobject<USceneComponent>("ApexComponent");
	ApexComponent->SetupAttachment(GetRootComponent());
}

void ALavaBall::BeginPlay()
{
	Super::BeginPlay();
	StartLaunching();
}

void ALavaBall::StartLaunching()
{
	InitialLocation = GetActorLocation();
	ApexLocation = ApexComponent->GetComponentLocation();
	LavaBallState = ELavaBallState::Idle;
	if(const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(LaunchTimer, this, &ALavaBall::LaunchTimeline, LaunchCoolDown, true);
}
