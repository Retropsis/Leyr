// @ Retropsis 2024-2025.

#include "World/Map/MapPoint.h"
#include "Game/LeyrGameMode.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AMapPoint::AMapPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void AMapPoint::LoadActor_Implementation()
{
	
}

void AMapPoint::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		bActivated = true;
		if (ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			LeyrGameMode->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}
