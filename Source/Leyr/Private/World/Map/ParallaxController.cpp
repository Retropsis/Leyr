// @ Retropsis 2024-2025.

#include "World/Map/ParallaxController.h"
#include "PaperSpriteComponent.h"
#include "Game/LeyrGameMode.h"
#include "Kismet/GameplayStatics.h"

#define CREATE_AND_SETUP_LAYER(LayerName, Priority, Depth) \
LayerName = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT(#LayerName)); \
LayerName->SetupAttachment(GetRootComponent()); \
LayerName->SetCollisionEnabled(ECollisionEnabled::NoCollision); \
LayerName->TranslucencySortPriority = Priority; \
LayerName->SetRelativeLocation(FVector(0.f, Depth, 0.f)); \
LayerName->SetGenerateOverlapEvents(false); \

AParallaxController::AParallaxController()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	CREATE_AND_SETUP_LAYER(Sky, -1000, -3000.f);
	CREATE_AND_SETUP_LAYER(Sun, 0, -2950.f);
	CREATE_AND_SETUP_LAYER(FirstMoon, 0, -2950.f);
	CREATE_AND_SETUP_LAYER(SecondMoon, 0, -2950.f);
	CREATE_AND_SETUP_LAYER(BackClouds, 0, -2900.f);
	CREATE_AND_SETUP_LAYER(FrontClouds, -900, -2800.f);
	CREATE_AND_SETUP_LAYER(BackLayer, 0, -2700.f);
	CREATE_AND_SETUP_LAYER(FrontLayer, -400, -2600.f);
	CREATE_AND_SETUP_LAYER(ForegroundFogLayer, 913, 200.f);
	CREATE_AND_SETUP_LAYER(BackgroundFogLayer, 0, -100.f);
	CREATE_AND_SETUP_LAYER(ForegroundLayer, -413, 100.f);
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

void AParallaxController::InitializeMapParallax()
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

