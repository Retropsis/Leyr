// @ Retropsis 2024-2025.

#include "World/Level/Moving/WaterGroup.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/PlayerCharacter.h"

AWaterGroup::AWaterGroup()
{
	OverlapBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	Tags.Add(FName("OverlapProjectiles"));
}

void AWaterGroup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	OverlapBox->SetBoxExtent(FVector(Width * 32.f, 100.f, Depth * 32.f));
	SurfaceZ = OverlapBox->GetComponentLocation().Z + OverlapBox->GetScaledBoxExtent().Z;
}

void AWaterGroup::BeginPlay()
{
	Super::BeginPlay();	
	CheckForPlayerOverlap();
}

void AWaterGroup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherComp && OtherComp->ComponentHasTag("SwimmingCollision"))
	{
		ActiveActors.AddUnique(OtherActor);
		if (OtherActor->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_HandleSwimming(OtherActor, MinZ, SwimmingSpeed, SwimmingGravityScale, false);
		}
		
		FVector Location{ OtherComp->GetComponentLocation().X, 0.f, OverlapBox->GetComponentLocation().Z + OverlapBox->GetScaledBoxExtent().Z + 32.f };
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, WaterSplash, Location);
	}
}

void AWaterGroup::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherComp && OtherComp->ComponentHasTag("SwimmingCollision") && ActiveActors.Contains(OtherActor))
	{
		ActiveActors.Remove(OtherActor);
		if (OtherActor->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_HandleSwimming(OtherActor, 0.f, 0.f, 0.f, true);
		}
		
		FVector Location{ OtherComp->GetComponentLocation().X, 0.f, OverlapBox->GetComponentLocation().Z + OverlapBox->GetScaledBoxExtent().Z + 32.f };
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, WaterSplash, Location);
	}
}

void AWaterGroup::HandleActiveActors(float DeltaSeconds)
{
	if(ActiveActors.Num() == 0) return;

	for (AActor* Actor : ActiveActors)
	{
		if(Actor)
		{
			const bool bSinkFaster = Actor->GetActorLocation().Z > SurfaceZ;
			
			FHitResult Hit;
			FVector Start = Actor->GetActorLocation();
			const FVector End = Start + FVector::DownVector * 55.f;
			UKismetSystemLibrary::SphereTraceSingle(Actor, Start, End, 18.f, TraceTypeQuery1, false, TArray<AActor*>(), EDrawDebugTrace::None, Hit, true);
			
			FVector CurrentBottom = Hit.bBlockingHit ? Hit.ImpactPoint : FVector(Start.X, 0.f, InterpTarget.Z);
			UKismetSystemLibrary::DrawDebugPoint(this, InterpTarget, 15.f, FLinearColor::Red);

			if (Hit.bBlockingHit) return;
			
			switch (MovingDirection) {
			case EMovingDirection::Sink:
				Actor->SetActorLocation(FMath::VInterpConstantTo(Start, CurrentBottom, DeltaSeconds, 10.f));
				UKismetSystemLibrary::DrawDebugPoint(this, CurrentBottom, 15.f, FLinearColor::Yellow);
				// Actor->SetActorLocation(FMath::VInterpTo(Actor->GetActorLocation(), CurrentBottom, DeltaSeconds, bSinkFaster ? 10.f : InterpolationSpeed));
				break;
			case EMovingDirection::Vector:
				UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + InterpTarget.GetSafeNormal(), 2.f, FLinearColor::Green);
				Actor->SetActorLocation(FMath::VInterpTo(Start, InterpTarget, DeltaSeconds, FlowingInterpSpeed));
				break;
			case EMovingDirection::Still:
				break;
			}
		}
	}
}

void AWaterGroup::CheckForPlayerOverlap()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EOT_PlayerCapsule);
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::BoxOverlapActors(this, GetActorLocation(), OverlapBox->GetScaledBoxExtent(), ObjectTypes, APlayerCharacter::StaticClass(), ActorsToIgnore, OutActors);
	for (AActor* OverlapActor : OutActors)
	{
		if(OverlapActor && OverlapActor->Implements<UPlayerInterface>())
		{
			ActiveActors.AddUnique(OverlapActor);
			IPlayerInterface::Execute_HandleSwimming(OverlapActor, MinZ, SwimmingSpeed, SwimmingGravityScale, false);
		}
	}
}