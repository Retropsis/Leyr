// @ Retropsis 2024-2025.

#include "World/Level/Ladder.h"

#include "Components/BoxComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"

ALadder::ALadder()
{
	BuildDirection = EBuildDirection::Vertical;
}

void ALadder::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		HangingCollision->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnBeginOverlap);
		HangingCollision->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnEndOverlap);
	}
}

void ALadder::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(160, 3.f, FColor::Green, FString::Printf(TEXT("OnBeginOverlap")));
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		FVector Location{HangingCollision->GetComponentLocation().X, 0.f, 0.f};
		IPlayerInterface::Execute_HandleHangingOnLadder(OtherActor, Location);
	}
}

void ALadder::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(161, 3.f, FColor::Red, FString::Printf(TEXT("OnEndOverlap")));
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(IgnoreCollisionTimer, this, &ALadder::HandleIgnoreCollisionEnd, IgnoreCollisionTime);
		}
	}
}

void ALadder::HandleIgnoreCollisionEnd()
{
	HangingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}