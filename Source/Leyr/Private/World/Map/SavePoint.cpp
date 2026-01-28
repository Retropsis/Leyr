// @ Retropsis 2024-2025.

#include "World/Map/SavePoint.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Game/LeyrGameMode.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Leyr/Leyr.h"

ASavePoint::ASavePoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("FlipbookComponent");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlipbookComponent->SetupAttachment(GetRootComponent());
	
	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapBox->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	OverlapBox->SetBoxExtent(FVector{ 32.f, 100.f, 32.f });
	OverlapBox->SetupAttachment(GetRootComponent());

	Tags.Add("SavePoint");
}

void ASavePoint::LoadActor_Implementation()
{
	if (bActivated)
	{
		// Do stuff
	}
}

void ASavePoint::BeginPlay()
{
	Super::BeginPlay();

	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ASavePoint::OnBeginOverlap);
}

void ASavePoint::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		bActivated = true;
		if (ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			const UWorld* World = GetWorld();
			FString MapName = World->GetMapName();
			MapName.RemoveFromStart(World->StreamingLevelsPrefix);
			LeyrGameMode->SaveWorldState(GetWorld(), MapName);
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
	}
}
