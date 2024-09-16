// @ Retropsis 2024-2025.

#include "World/Level/Door/Door.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "World/Level/Lever.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetRenderComponent()->SetLooping(false);
	
	DoorCollision = CreateDefaultSubobject<UBoxComponent>("DoorCollision");
	DoorCollision->SetupAttachment(GetRootComponent());
	
	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	OverlapBox->SetupAttachment(GetRootComponent());
	
	Background = CreateDefaultSubobject<UPaperSpriteComponent>("Background");
	Background->SetupAttachment(GetRootComponent());
	
	Foreground = CreateDefaultSubobject<UPaperSpriteComponent>("Foreground");
	Foreground->SetupAttachment(GetRootComponent());
	
	OpenPosition = CreateDefaultSubobject<USceneComponent>("OpenPosition");
	OpenPosition->SetupAttachment(GetRootComponent());
}

void ADoor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(DoorState == EDoorState::Opening || DoorState == EDoorState::Closing) HandleDoorMoving(DeltaSeconds);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	GetRenderComponent()->OnFinishedPlaying.AddDynamic(this, &ADoor::HandleOnFinishedPlaying);
	GetRenderComponent()->SetPlaybackPosition(0.f, true);
	
	if(IsValid(Switch))
	{
		bIsProximityDoor = false;
		Switch->OnLeverStateChanged.AddLambda([this] (const ELeverState NewState)
		{
			HandleDoorState(NewState == ELeverState::On);
		});
	}
	
	if (HasAuthority() && bIsProximityDoor)
	{
		OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnBeginOverlap);
		OverlapBox->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnEndOverlap);
	}
}

void ADoor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	GetRenderComponent()->SetPlaybackPosition(DoorState == EDoorState::Close ? 0.f : 1.f, true);
	CloseLocation = DoorCollision->GetComponentLocation();
	OpenLocation = OpenPosition->GetComponentLocation();
}

void ADoor::HandleDoorMoving(float DeltaSeconds) const
{
	const FVector TargetLocation = DoorState == EDoorState::Opening ? OpenLocation : CloseLocation;
	DoorCollision->SetWorldLocation(FMath::VInterpTo(DoorCollision->GetComponentLocation(), TargetLocation, DeltaSeconds, 5.f));
}

void ADoor::HandleOnFinishedPlaying()
{
	GetRenderComponent()->Stop();
	switch (DoorState) {
	case EDoorState::Still:
		break;
	case EDoorState::Opening:
		DoorState = EDoorState::Open;
		break;
	case EDoorState::Closing:
		DoorState = EDoorState::Close;
		// TODO: Niagara Smoke & Sounds
		break;
	case EDoorState::Open:
		break;
	case EDoorState::Close:
		break;
	}
}

void ADoor::HandleDoorState(bool bOpen)
{
	DoorState = bOpen ? EDoorState::Opening : EDoorState::Closing;
	GetRenderComponent()->SetPlayRate(1.f);
	bOpen ? GetRenderComponent()->Play() : GetRenderComponent()->Reverse();
	
	if(!bOpen) GetRenderComponent()->SetPlaybackPosition(GetRenderComponent()->GetFlipbookLength(), true);
}

void ADoor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->ActorHasTag("Player")) HandleDoorState(true);
}

void ADoor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor->ActorHasTag("Player"))HandleDoorState(false);
}

