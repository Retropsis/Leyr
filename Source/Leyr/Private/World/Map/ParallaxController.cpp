// @ Retropsis 2024-2025.

#include "World/Map/ParallaxController.h"
#include "PaperSpriteComponent.h"
#include "Game/LeyrGameMode.h"
#include "Kismet/GameplayStatics.h"

AParallaxController::AParallaxController()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	Sky = CreateDefaultSubobject<UPaperSpriteComponent>("Sky");
	Sky->SetupAttachment(GetRootComponent());
	Sky->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sky->TranslucencySortPriority = -1000;
	Sky->SetRelativeLocation(FVector(0.f, -1000.f, 0.f));
	
	Sun = CreateDefaultSubobject<UPaperSpriteComponent>("Sun");
	Sun->SetupAttachment(Sky);
	Sun->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sun->SetRelativeLocation(FVector(0.f, 50.f, 0.f));
	
	FirstMoon = CreateDefaultSubobject<UPaperSpriteComponent>("FirstMoon");
	FirstMoon->SetupAttachment(Sky);
	FirstMoon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FirstMoon->SetRelativeLocation(FVector(0.f, 50.f, 0.f));
	
	SecondMoon = CreateDefaultSubobject<UPaperSpriteComponent>("SecondMoon");
	SecondMoon->SetupAttachment(Sky);
	SecondMoon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SecondMoon->SetRelativeLocation(FVector(0.f, 50.f, 0.f));
	
	BackClouds = CreateDefaultSubobject<UPaperSpriteComponent>("BackClouds");
	BackClouds->SetupAttachment(Sky);
	BackClouds->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BackClouds->SetRelativeLocation(FVector(0.f, 100.f, 0.f));
	
	FrontClouds = CreateDefaultSubobject<UPaperSpriteComponent>("FrontClouds");
	FrontClouds->SetupAttachment(Sky);
	FrontClouds->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FrontClouds->TranslucencySortPriority = -900;
	FrontClouds->SetRelativeLocation(FVector(0.f, 200.f, 0.f));
	
	BackLayer = CreateDefaultSubobject<UPaperSpriteComponent>("BackLayer");
	BackLayer->SetupAttachment(Sky);
	BackLayer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BackLayer->SetRelativeLocation(FVector(0.f, 300.f, 0.f));
	
	FrontLayer = CreateDefaultSubobject<UPaperSpriteComponent>("FrontLayer");
	FrontLayer->SetupAttachment(Sky);
	FrontLayer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FrontLayer->TranslucencySortPriority = -400;
	FrontLayer->SetRelativeLocation(FVector(0.f, 400.f, 0.f));
	
	ForegroundFogLayer = CreateDefaultSubobject<UPaperSpriteComponent>("ForegroundFogLayer");
	ForegroundFogLayer->SetupAttachment(Sky);
	ForegroundFogLayer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ForegroundFogLayer->TranslucencySortPriority = 900;
	ForegroundFogLayer->SetRelativeLocation(FVector(0.f, 1200.f, 0.f));
	
	BackgroundFogLayer = CreateDefaultSubobject<UPaperSpriteComponent>("BackgroundFogLayer");
	BackgroundFogLayer->SetupAttachment(Sky);
	BackgroundFogLayer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BackgroundFogLayer->SetRelativeLocation(FVector(0.f, 900.f, 0.f));
	
	ForegroundLayer = CreateDefaultSubobject<UPaperSpriteComponent>("ForegroundLayer");
	ForegroundLayer->SetupAttachment(Sky);
	ForegroundLayer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ForegroundLayer->TranslucencySortPriority = -400;
	ForegroundLayer->SetRelativeLocation(FVector(0.f, 1100.f, 0.f));
}

void AParallaxController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	DeltaSecondsNormalized = DeltaSecondsNormalized + DeltaSeconds;
	if(DynamicLayersToSpeed.Num() > 0)
	{
		for (TTuple<UMaterialInstanceDynamic*, FParallaxLayerData> Data : DynamicLayersToSpeed)
		{
			if(Data.Key)
			{
				Data.Key->SetScalarParameterValue(FName("CameraPosX"), GetActorLocation().X * Data.Value.ScrollingRatio.X + Data.Value.ScrollingSpeed != 0.f ? Data.Value.ScrollingSpeed * DeltaSecondsNormalized : 0.f);
				if(Data.Value.ScrollingRatio.Y != 0.f) Data.Key->SetScalarParameterValue(FName("CameraPosY"), GetActorLocation().Z * Data.Value.ScrollingRatio.Y);
				// if(Data.Value.ScrollingRatio.Y != 0.f) Data.Key->SetScalarParameterValue(FName("CameraPosY"), 1.f / FMath::Abs(GetActorLocation().Z * Data.Value.ScrollingRatio.Y));
				if(Data.Value.OffsetY != 0.f) Data.Key->SetScalarParameterValue(FName("OffsetY"), Data.Value.OffsetY);
			}
		}
	}
}

void AParallaxController::InitializeMapParallax(AActor* ActorToAttach)
{
	if(!HasAuthority()) return;

	const ALeyrGameMode* LeyrGameMode = Cast<ALeyrGameMode>(UGameplayStatics::GetGameMode(this));
	if (LeyrGameMode == nullptr || CurrentMapName.IsNone()) return;
	
	const FMapDefaultInfo Info = LeyrGameMode->MapInfo->GetMapDefaultInfo(CurrentMapName);
	DynamicLayersToSpeed.Empty();

	if(Info.SkyData.Sprite) Sky->SetSprite(Info.SkyData.Sprite);
	if(Info.SunData.Sprite && Info.SunData.bIsVisible)
	{
		Sun->SetSprite(Info.SunData.Sprite);
		SunSpeed = Info.SunData.ScrollingRatio;
		DynamicLayersToSpeed.Add(Sun->CreateDynamicMaterialInstance(0), Info.SunData);
	}
	if(Info.FirstMoonData.Sprite && Info.FirstMoonData.bIsVisible)
	{
		FirstMoon->SetSprite(Info.FirstMoonData.Sprite);
		FirstMoonSpeed = Info.FirstMoonData.ScrollingRatio;
		DynamicLayersToSpeed.Add(FirstMoon->CreateDynamicMaterialInstance(0), Info.FirstMoonData);
	}
	if(Info.SecondMoonData.Sprite && Info.SecondMoonData.bIsVisible)
	{
		SecondMoon->SetSprite(Info.SecondMoonData.Sprite);
		SecondMoonSpeed = Info.SecondMoonData.ScrollingRatio;
		DynamicLayersToSpeed.Add(SecondMoon->CreateDynamicMaterialInstance(0), Info.SecondMoonData);
	}
	if(Info.FrontCloudsData.Sprite && Info.FrontCloudsData.bIsVisible)
	{
		FrontClouds->SetSprite(Info.FrontCloudsData.Sprite);
		FrontCloudsSpeed = Info.FrontCloudsData.ScrollingRatio;
		DynamicLayersToSpeed.Add(FrontClouds->CreateDynamicMaterialInstance(0), Info.FrontCloudsData);
	}
	if(Info.BackCloudsData.Sprite && Info.BackCloudsData.bIsVisible)
	{
		BackClouds->SetSprite(Info.BackCloudsData.Sprite);
		BackCloudsSpeed = Info.BackCloudsData.ScrollingRatio;
		DynamicLayersToSpeed.Add(BackClouds->CreateDynamicMaterialInstance(0), Info.BackCloudsData);
	}
	if(Info.FrontLayerData.Sprite && Info.FrontLayerData.bIsVisible)
	{
		FrontLayer->SetSprite(Info.FrontLayerData.Sprite);
		FrontLayerSpeed = Info.FrontLayerData.ScrollingRatio;
		DynamicLayersToSpeed.Add(FrontLayer->CreateDynamicMaterialInstance(0), Info.FrontLayerData);
	}
	if(Info.BackLayerData.Sprite && Info.BackLayerData.bIsVisible)
	{
		BackLayer->SetSprite(Info.BackLayerData.Sprite);
		BackLayerSpeed = Info.BackLayerData.ScrollingRatio;
		DynamicLayersToSpeed.Add(BackLayer->CreateDynamicMaterialInstance(0), Info.BackLayerData);
	}
	if(Info.BackgroundFogLayerData.Sprite && Info.BackgroundFogLayerData.bIsVisible)
	{
		BackgroundFogLayer->SetSprite(Info.BackgroundFogLayerData.Sprite);
		BackgroundFogLayerSpeed = Info.BackgroundFogLayerData.ScrollingRatio;
		DynamicLayersToSpeed.Add(BackgroundFogLayer->CreateDynamicMaterialInstance(0), Info.BackgroundFogLayerData);
	}
	if(Info.ForegroundFogLayerData.Sprite && Info.ForegroundFogLayerData.bIsVisible)
	{
		ForegroundFogLayer->SetSprite(Info.ForegroundFogLayerData.Sprite);
		ForegroundFogLayerSpeed = Info.ForegroundFogLayerData.ScrollingRatio;
		DynamicLayersToSpeed.Add(ForegroundFogLayer->CreateDynamicMaterialInstance(0), Info.ForegroundFogLayerData);
	}
	if(Info.ForegroundLayerData.Sprite && Info.ForegroundLayerData.bIsVisible)
	{
		ForegroundLayer->SetSprite(Info.ForegroundLayerData.Sprite);
		ForegroundLayerSpeed = Info.ForegroundLayerData.ScrollingRatio;
		DynamicLayersToSpeed.Add(ForegroundLayer->CreateDynamicMaterialInstance(0), Info.ForegroundLayerData);
	}
}

