// @ Retropsis 2024-2025.

#include "World/Level/PCA_Brazier.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

void APCA_Brazier::BeginPlay()
{
	Super::BeginPlay();
	WeightBoxComponent->OnComponentHit.AddDynamic(this, &APCA_Brazier::OnHit);
	WeightBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APCA_Brazier::OnBeginOverlap);
}

void APCA_Brazier::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		WeightBoxComponent->AddImpulse(OtherActor->GetActorForwardVector() * ImpactImpulse, NAME_None, true);
	}
}

void APCA_Brazier::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bHit)
	{
		const FVector HitLocation{ Hit.Location.X, 5.f, Hit.Location.Z + 75.f };
		if (ImpactEffect->IsValid()) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, HitLocation);
		if (IsValid(ImpactSound)) UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, HitLocation);
		SetLifeSpan(.5f);
		bHit = true;
	}
}
