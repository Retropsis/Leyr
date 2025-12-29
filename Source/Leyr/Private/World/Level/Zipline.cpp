// @ Retropsis 2024-2025.

#include "World/Level/Zipline.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Leyr/Leyr.h"

AZipline::AZipline()
{
	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	OverlapBox->SetupAttachment(GetRootComponent());
	bIsActivated = false;
	CurrentIndex = 1;
}

void AZipline::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(BoxCollision->IsGravityEnabled()) BoxCollision->SetRelativeRotation(FRotator::ZeroRotator);
}

void AZipline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	BoxCollision->SetBoxExtent(FVector(16.f * Length, 100.f, 16.f));
}

void AZipline::BeginPlay()
{
	AActor::BeginPlay();
	CurrentTarget = RouteSpline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
	if (HasAuthority())
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AZipline::OnBeginOverlap);
		BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AZipline::OnEndOverlap);
	}
}

void AZipline::ChooseNextStep()
{
	bHasReachedTarget = true;
	if(bIsSingleUse) SetLifeSpan(3.f);
	
	if(bShouldCollapse)
	{
		GetWorld()->GetTimerManager().SetTimer(FallingTimer, this, &AZipline::HandleFallingTimeEnd, FallingTime);
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		BoxCollision->SetSimulatePhysics(true);
		BoxCollision->SetEnableGravity(true);
	}
	else if (bHasEndedOverlap)
	{
		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AZipline::HandleRespawnTimeEnd, RespawnTime);
	}
	
	if(OverlapWaitTimer.IsValid()) OverlapWaitTimer.Invalidate();
}

void AZipline::ResetState_Implementation()
{
	Super::ResetState_Implementation();
	BoxCollision->SetRelativeLocation(FVector::ZeroVector);
	bIsActivated = false;
	bHasReachedTarget = false;
	bHasEndedOverlap = false;
	CurrentIndex = 1;
}

void AZipline::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherComp && OtherComp->ComponentHasTag("RopeCollision"))
	{
		GetWorld()->GetTimerManager().SetTimer(OverlapWaitTimer, this, &AZipline::HandleOverlapWaitTimeEnd, OverlapWaitTime);
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		const FVector Location{ BoxCollision->GetComponentLocation().X, 0.f, BoxCollision->GetComponentLocation().Z };
		if(OtherActor && !OtherActor->IsAttachedTo(this)) OtherActor->AttachToComponent(BoxCollision, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		IPlayerInterface::Execute_HandleHangingOnHook(OtherActor, Location, false);
	}
}

void AZipline::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherComp && OtherComp->ComponentHasTag("RopeCollision"))
	{
		IPlayerInterface::Execute_HandleHangingOnHook(OtherActor, FVector::ZeroVector, true);
		bHasEndedOverlap = true;
		if (bHasReachedTarget)
		{
			GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AZipline::HandleRespawnTimeEnd, RespawnTime);
		}
	}
}

void AZipline::HandleFallingTimeEnd()
{
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AZipline::HandleRespawnTimeEnd, RespawnTime);
	if(FallingTimer.IsValid()) FallingTimer.Invalidate();
	OnTrolleyFalling.Broadcast();
}

void AZipline::HandleOverlapWaitTimeEnd()
{
	bIsActivated = true;
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AZipline::HandleRespawnTimeEnd()
{
	BoxCollision->SetSimulatePhysics(false);
	bIsActivated = false;
	CurrentIndex = 1;
	CurrentTarget = RouteSpline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
	BoxCollision->SetWorldLocation(RouteSpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World));
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollision->SetEnableGravity(false);
	OverlapBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapBox->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	OverlapBox->SetCollisionResponseToChannel(ECC_Enemy, ECR_Overlap);
	if(RespawnTimer.IsValid()) RespawnTimer.Invalidate();
	OnTrolleyRespawn.Broadcast();
}
