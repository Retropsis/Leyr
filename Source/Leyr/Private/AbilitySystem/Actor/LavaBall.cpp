// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/LavaBall.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

ALavaBall::ALavaBall()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALavaBall::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(GetVelocity().Z < 0.f && GetActorLocation().Z <= StoppingHeight)
	{
		BoxCollision->SetSimulatePhysics(false);
		BoxCollision->SetEnableGravity(0.f);
		bLanded = true;
		GEngine->AddOnScreenDebugMessage(35777, 3.f, FColor::Purple, FString("Landed"));
	}
}

void ALavaBall::BeginPlay()
{
	Super::BeginPlay();
	if(const UWorld* World = GetWorld()) World->GetTimerManager().SetTimer(LaunchTimer, this, &ALavaBall::Launch, LaunchCoolDown, true);
	StoppingHeight = GetActorLocation().Z + 1.f;
}

void ALavaBall::Launch()
{
	BoxCollision->SetSimulatePhysics(true);
	BoxCollision->SetEnableGravity(1.f);
	bLanded = false;
	BoxCollision->AddImpulse(FVector(0.f, 0., LaunchImpulse), NAME_None, true);
}
