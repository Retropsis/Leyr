// @ Retropsis 2024-2025.

#include "World/Level/Hanger.h"
#include "PaperGroupedSpriteComponent.h"
#include "Components/BoxComponent.h"

AHanger::AHanger()
{
	PrimaryActorTick.bCanEverTick = false;
	
	HangingCollision = CreateDefaultSubobject<UBoxComponent>("HangingCollision");
	SetRootComponent(HangingCollision);
	
	TileMap = CreateDefaultSubobject<UPaperGroupedSpriteComponent>("TileMap");
	TileMap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TileMap->SetupAttachment(HangingCollision);
}

void AHanger::BuildTileMap()
{
	if (BuildDirection == EBuildDirection::None) return;
	if (Tiles.Num() == 0 || Tiles[0] == nullptr) return;
	
	for (int i = 0; i < Length - 1; ++i)
	{
		FTransform Transform = FTransform();
		const int32 Selection = FMath::RandRange(0, Tiles.Num() - 1);
		UPaperSprite* ChosenTile = Tiles[Selection];
		
		if (i == 0) ChosenTile = FirstTile ?  FirstTile : ChosenTile;
		
		else if (i == Length - 1) ChosenTile = LastTile ?  LastTile : ChosenTile;

		FVector Location = FVector::ZeroVector;
		switch (BuildDirection) {
		case EBuildDirection::Horizontal: Location = FVector(i * 96.f, 0.f, 0.f);
			break;
		case EBuildDirection::Vertical: Location = FVector(0.f, 0.f, i * 96.f);
			break;
		default: ;
		}
		Transform.SetLocation(Location);
		if(ChosenTile) TileMap->AddInstance(Transform, ChosenTile);
	}
	
	FVector Extents = FVector::ZeroVector;
	switch (BuildDirection) {
	case EBuildDirection::Horizontal: Extents = FVector(Length * 96.f, 100.f, 16.f);
		break;
	case EBuildDirection::Vertical: Extents = FVector(96.f, 100.f, Length * 96.f);
		break;
	default: ;
	}
	HangingCollision->SetBoxExtent(Extents);
}

