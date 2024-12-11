// @ Retropsis 2024-2025.

#include "World/Level/Elevator/Elevator.h"
#include "Components/BoxComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Interaction/PlayerInterface.h"
#include "Leyr/Leyr.h"

AElevator::AElevator()
{
	PrimaryActorTick.bCanEverTick = false;

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");
	
	FloorCollision = CreateDefaultSubobject<UBoxComponent>("FloorCollision");
	FloorCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	FloorCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	FloorCollision->SetCollisionResponseToChannel(ECC_Player, ECR_Block);
	SetRootComponent(FloorCollision);
	
	CeilingCollision = CreateDefaultSubobject<UBoxComponent>("CeilingCollision");
	CeilingCollision->SetupAttachment(GetRootComponent());
	CeilingCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	CeilingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CeilingCollision->SetCollisionResponseToChannel(ECC_Player, ECR_Block);
	
	OccupancyCollision = CreateDefaultSubobject<UBoxComponent>("OccupancyCollision");
	OccupancyCollision->SetupAttachment(GetRootComponent());
	OccupancyCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	OccupancyCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OccupancyCollision->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	
	TopPosition = CreateDefaultSubobject<USceneComponent>("TopPosition");
	TopPosition->SetupAttachment(GetRootComponent());
	
	BottomPosition = CreateDefaultSubobject<USceneComponent>("BottomPosition");
	BottomPosition->SetupAttachment(GetRootComponent());
}

void AElevator::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		OccupancyCollision->OnComponentBeginOverlap.AddDynamic(this, &AElevator::OnBeginOverlap);
		OccupancyCollision->OnComponentEndOverlap.AddDynamic(this, &AElevator::OnEndOverlap);
		TopPositionZ = TopPosition->GetComponentLocation().Z;
		BottomPositionZ = BottomPosition->GetComponentLocation().Z;
	}
}

void AElevator::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		if(OtherActor && !OtherActor->IsAttachedTo(this)) OtherActor->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		IPlayerInterface::Execute_HandleElevator(OtherActor, this, false);
	}
}

void AElevator::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		if(OtherActor && OtherActor->GetAttachParentActor()) OtherActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		IPlayerInterface::Execute_HandleElevator(OtherActor, nullptr, true);
	}
}

void AElevator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AElevator::Move_Implementation(float ScaleValue)
{
	if(GetActorLocation().Z > BottomPositionZ && ScaleValue < 0.f)
	{
		AddMovementInput(FVector::UpVector, ScaleValue);
	}
	if(GetActorLocation().Z < TopPositionZ && ScaleValue > 0.f)
	{
		AddMovementInput(FVector::UpVector, ScaleValue);
	}
}

