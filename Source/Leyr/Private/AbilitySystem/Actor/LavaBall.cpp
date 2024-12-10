// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/LavaBall.h"
#include "Components/BoxComponent.h"

ALavaBall::ALavaBall()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALavaBall::BeginPlay()
{
	Super::BeginPlay();
	if(const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(LaunchTimer, this, &ALavaBall::Launch, LaunchCoolDown, true);
	BoxCollision->SetSimulatePhysics(true);
}

void ALavaBall::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(bShouldRotate && GetVelocity().Z < 0.f)
	{
		ShouldRotate(GetVelocity().Z < 0.f);
		bShouldRotate = false;
	}
}

void ALavaBall::Launch()
{
	BoxCollision->AddImpulse(FVector(0.f, 0., LaunchImpulse), NAME_None, true);
	ShouldRotate(false);
	bShouldRotate = true;
}
