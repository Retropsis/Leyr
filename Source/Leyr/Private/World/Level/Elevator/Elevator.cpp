// @ Retropsis 2024-2025.

#include "World/Level/Elevator/Elevator.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Leyr/Leyr.h"

AElevator::AElevator()
{
	PrimaryActorTick.bCanEverTick = false;
	FloorCollision = CreateDefaultSubobject<UBoxComponent>("FloorCollision");
	SetRootComponent(FloorCollision);
	
	CeilingCollision = CreateDefaultSubobject<UBoxComponent>("CeilingCollision");
	CeilingCollision->SetupAttachment(GetRootComponent());
	
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
		IPlayerInterface::Execute_HandleElevator(OtherActor, this, false);
	}
}

void AElevator::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_HandleElevator(OtherActor, nullptr, true);
	}
}

void AElevator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AElevator::Move_Implementation(float ScaleValue)
{
	AddMovementInput(FVector::UpVector, ScaleValue);
}

