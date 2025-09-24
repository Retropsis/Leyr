// @ Retropsis 2024-2025.

#include "World/Map/CameraBoundary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "PaperTileMapComponent.h"
#include "AI/AICharacter.h"
#include "Data/LevelAreaData.h"
#include "Engine/OverlapResult.h"
#include "Interaction/LevelActorInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "World/Data/CameraData.h"
#include "World/Level/Spawner/EncounterSpawnVolume.h"

ACameraBoundary::ACameraBoundary()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	EnteringBoundary = CreateDefaultSubobject<UBoxComponent>("Entering Boundary");
	EnteringBoundary->InitBoxExtent(FVector(50.f));
	EnteringBoundary->SetupAttachment(GetRootComponent());
	EnteringBoundary->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EnteringBoundary->SetCollisionResponseToAllChannels(ECR_Ignore);
	EnteringBoundary->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	EnteringBoundary->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	
	NavigationBoundary = CreateDefaultSubobject<UBoxComponent>("Navigation Boundary");
	NavigationBoundary->InitBoxExtent(FVector(50.f));
	NavigationBoundary->SetupAttachment(GetRootComponent());
	NavigationBoundary->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	NavigationBoundary->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	CameraBoundary = CreateDefaultSubobject<UBoxComponent>("Camera Extents");
	CameraBoundary->InitBoxExtent(FVector(50.f));
	CameraBoundary->SetupAttachment(GetRootComponent());
	CameraBoundary->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CameraBoundary->SetHiddenInGame(true);

	BoundaryVisualizer = CreateDefaultSubobject<UStaticMeshComponent>("Entering Boundary Visualizer");
	BoundaryVisualizer->SetupAttachment(GetRootComponent());
	BoundaryVisualizer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoundaryVisualizer->SetHiddenInGame(true);
	BoundaryVisualizer->bCastDynamicShadow = false;
}

void ACameraBoundary::BeginPlay()
{
	Super::BeginPlay();
	EnteringBoundary->OnComponentBeginOverlap.AddDynamic(this, &ACameraBoundary::OnBeginOverlap);
	EnteringBoundary->OnComponentEndOverlap.AddDynamic(this, &ACameraBoundary::OnEndOverlap);
	
	for (const TObjectPtr<AEncounterSpawnVolume> SpawningVolume : SpawningVolumes)
	{
		if (SpawningVolume == nullptr) continue;
		OnPlayerLeaving.AddUObject(SpawningVolume, &AEncounterSpawnVolume::HandlePlayerLeaving);
		OnPlayerEntering.AddUObject(SpawningVolume, &AEncounterSpawnVolume::HandlePlayerEntering);
	}
}

void ACameraBoundary::InitializeCameraExtent()
{
	if (TileMap)
	{
		SetActorLocation(TileMap->GetRenderComponent()->Bounds.Origin);
		TileMapBounds = TileMap->GetRenderComponent()->Bounds;
		EnteringBoundary->SetBoxExtent(FVector{ TileMapBounds.BoxExtent.X - 18.f, TileMapBounds.BoxExtent.Y, TileMapBounds.BoxExtent.Z - 54.f });
		NavigationBoundary->SetBoxExtent(FVector{ TileMapBounds.BoxExtent.X - 128.f, TileMapBounds.BoxExtent.Y, TileMapBounds.BoxExtent.Z - 128.f });
		BoundaryVisualizer->SetWorldScale3D(FVector{ TileMapBounds.BoxExtent.X / 50.f, TileMapBounds.BoxExtent.Y / 50.f, TileMapBounds.BoxExtent.Z / 50.f });
		CameraBoundary->SetBoxExtent(FVector{ bConstrainX ? FMath::Max(0.f, TileMapBounds.BoxExtent.X - 640.f) : TileMapBounds.BoxExtent.X, 0.f, bConstrainZ ? FMath::Max(0.f, TileMapBounds.BoxExtent.Z - 384.f) : TileMapBounds.BoxExtent.Z });
	}
}

void ACameraBoundary::InitializeSpawnVolumes()
{	
	if (SpawningVolumeClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Please fill up SpawningVolumeClass in [%s]"), *GetName());
		return;
	}
	if (LevelAreaData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Please fill up LevelAreaData in [%s]"), *GetName());
		return;
	}
	
	ClearSpawnVolumes();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
	FVector Offset = FVector{ 100.f, 0.f, 0.f };
	for (int i = 0; i < LevelAreaData->EncounterSpawns.Num(); i++)
	{
		FVector Location = FVector{ GetActorLocation().X, 0.f, GetActorLocation().Z } + Offset;
		AEncounterSpawnVolume* SpawningVolume = GetWorld()->SpawnActor<AEncounterSpawnVolume>(SpawningVolumeClass, Location, FRotator::ZeroRotator, SpawnParams);
		SpawningVolume->SetEncounterSpawnData(LevelAreaData->EncounterSpawns[i]);
		SpawningVolume->InitializeSpawnPoints();
		SpawningVolume->TileMapBounds = TileMapBounds;
		SpawningVolumes.Add(SpawningVolume);
		Offset.X += 50.f;
	}
}

void ACameraBoundary::ClearSpawnVolumes()
{
	if (SpawningVolumes.Num() == 0) return;

	for (AEncounterSpawnVolume* SpawningVolume : SpawningVolumes)
	{
		if (IsValid(SpawningVolume))
		{
			SpawningVolume->ClearSpawnPoints();
			SpawningVolume->Destroy();
		}
	}
	OnPlayerLeaving.RemoveAll(this);
	SpawningVolumes.Empty();
}

void ACameraBoundary::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		HandleOnBeginOverlap(OtherActor);
		OnPlayerEntering.Broadcast();
		ToggleLevelActorActivity(true);
		GiveToAbilitySystem(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor), &LevelArea_GrantedHandles);
	}
}

/*
 * Encounters from the previous Boundary are destroyed when the player leaves
 * (doesn't handle case where encounter destroyed if itself leaves)
 */
void ACameraBoundary::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor->Implements<UPlayerInterface>() && OtherActor->Implements<UCombatInterface>() && ICombatInterface::Execute_GetDefeatState(OtherActor) == EDefeatState::None)
	{
		OnPlayerLeaving.Broadcast();
		// DestroyOutOfBoundsEncounters();
		ToggleLevelActorActivity(false);
		LevelArea_GrantedHandles.TakeFromAbilitySystem(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor));
	}
}

void ACameraBoundary::HandleOnBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_SetCameraInterpolation(OtherActor, this, ECameraInterpState::Entering);
	}
}

void ACameraBoundary::DestroyOutOfBoundsEncounters() const
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EOT_EnemyCapsule);
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::BoxOverlapActors(this, GetActorLocation(), EnteringBoundary->GetScaledBoxExtent(), ObjectTypes, AAICharacter::StaticClass(), ActorsToIgnore, OutActors);
	for (AActor* OverlapActor : OutActors)
	{
		if (IsValid(OverlapActor) && OverlapActor->Implements<UAIInterface>() && IAIInterface::Execute_ShouldDespawn(OverlapActor))
		{
			OverlapActor->Destroy();
		}
	}
}

void ACameraBoundary::ToggleLevelActorActivity(bool bActivate) const
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EOT_OneWayPlatform);
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;
	FCollisionQueryParams BoxParams;
	BoxParams.AddIgnoredActors(ActorsToIgnore);
	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeBox(EnteringBoundary->GetScaledBoxExtent()), BoxParams);
	for (FOverlapResult& Overlap : Overlaps)
	{
		if (Overlap.GetActor() && Overlap.GetActor()->Implements<ULevelActorInterface>())
		{
			ILevelActorInterface::Execute_ToggleActivate(Overlap.GetActor(), bActivate);
			if (bActivate) ILevelActorInterface::Execute_ResetState(Overlap.GetActor());
		}
	}
}

void ACameraBoundary::GiveToAbilitySystem(UAbilitySystemComponent* ASC, FLevelArea_GrantedHandles* OutGrantedHandles, float Level, UObject* SourceObject) const
{
	check(ASC);

	if (!ASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	// Grant the gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FLevelArea_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		FGameplayEffectContextHandle ContextHandle =ASC->MakeEffectContext();
		ContextHandle.AddSourceObject(ASC->GetAvatarActor());
		const FActiveGameplayEffectHandle GameplayEffectHandle = ASC->ApplyGameplayEffectToSelf(GameplayEffect, Level, ContextHandle);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}
}

void FLevelArea_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FLevelArea_GrantedHandles::TakeFromAbilitySystem(UAbilitySystemComponent* ASC)
{
	check(ASC);

	if (!ASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(Handle);
		}
	}
	GameplayEffectHandles.Reset();
}