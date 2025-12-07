// @ Retropsis 2024-2025.

#include "World/Map/CameraBoundary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "PaperTileMapComponent.h"
#include "AI/AICharacter.h"
#include "Data/LevelAreaData.h"
#include "Engine/OverlapResult.h"
#include "Interaction/InteractionInterface.h"
#include "Interaction/LevelActorInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "World/Item.h"
#include "World/Data/CameraData.h"
#include "World/Level/Spawner/EncounterSpawnPoint.h"
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

void ACameraBoundary::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	/*
	 * Opening the map in editor is calling it as well.
	 */
	if (LevelAreaData != nullptr)
	{
		LevelAreaData->OnLevelAreaDataPropertyChanged.AddLambda([this] (FName PropertyName) {
			UE_LOG(LogTemp, Warning, TEXT("Level Area Data Property Changed : %s"), *PropertyName.ToString());
			UpdateSpawnVolumes();
		});
		LevelAreaData->OnEncounterSpawnsPropertyDeleted.AddLambda([this] (const FGameplayTag Tag) {
			for (TObjectPtr<AEncounterSpawnVolume> SpawnVolume : SpawningVolumes)
			{
				if (IsValid(SpawnVolume) && SpawnVolume->GetEncounterSpawnTag().MatchesTagExact(Tag))
				{
					UE_LOG(LogTemp, Warning, TEXT("SpawnVolume with tag : %s, deleting with tag: %s"), *SpawnVolume->GetEncounterSpawnTag().ToString(), *Tag.ToString());
					SpawningVolumes.Remove(SpawnVolume);
					SpawnVolume->ClearSpawnPoints();
					SpawnVolume->Destroy();
					break;
				}
			}
		});
	}
}

void ACameraBoundary::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.Property->GetName() == TEXT("TileMap"))
	{
		InitializeCameraExtent();
	}
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

	if (LevelAreaData != nullptr)
	{
		EnvironmentEffects.Append(LevelAreaData->EnvironmentEffects);
	}
}

void ACameraBoundary::InitializeCameraExtent()
{
	if (TileMap)
	{
		TileMapBounds = GetTileMapBounds();
		SetActorLocation(TileMapBounds.Origin);
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
		const FString NewLabel = LevelAreaData->EncounterSpawns[i].EncounterClass->GetName().Replace(TEXT("BP"), TEXT("SpawningVolume"));
		CreateSpawningVolume(SpawnParams, LevelAreaData->EncounterSpawns[i], Offset, NewLabel);
		Offset.X += 50.f;
	}
}

void ACameraBoundary::CreateSpawningVolume(const FActorSpawnParameters& SpawnParams, const FEncounterSpawn& Data, const FVector& Offset, const FString& Label)
{
	FVector Location = FVector{ GetActorLocation().X, 0.f, GetActorLocation().Z } + Offset;
	AEncounterSpawnVolume* SpawningVolume = GetWorld()->SpawnActor<AEncounterSpawnVolume>(SpawningVolumeClass, Location, FRotator::ZeroRotator, SpawnParams);
	SpawningVolume->SetEncounterSpawnData(Data);
	SpawningVolume->SetEncounterSpawnTag(Data.EncounterSpawnTag);
	SpawningVolume->InitializeSpawnPoints();
	SpawningVolume->TileMapBounds = GetTileMapBounds();
	SpawningVolume->SetActorLabel(Label);
	SpawningVolumes.Add(SpawningVolume);
}

void ACameraBoundary::UpdateSpawnVolumes()
{
	if (SpawningVolumes.Num() == 0) return;

	if (SpawningVolumes.Num() < LevelAreaData->EncounterSpawns.Num())
	{
		for (int i = SpawningVolumes.Num(); i < LevelAreaData->EncounterSpawns.Num(); ++i)
		{
			// Create a new spawn volume
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			FVector Offset = FVector{ 100.f + 50.f * i, 0.f, 0.f };
			const FString NewLabel = LevelAreaData->EncounterSpawns[i].EncounterClass->GetName().Replace(TEXT("BP"), TEXT("SpawningVolume"));
			CreateSpawningVolume(SpawnParams, LevelAreaData->EncounterSpawns[i], Offset, NewLabel);
		}
		return;
	}

	for (int i = 0; i < SpawningVolumes.Num(); ++i)
	{
		if (IsValid(SpawningVolumes[i]))
		{
			SpawningVolumes[i]->SetEncounterSpawnData(LevelAreaData->EncounterSpawns[i]);
			SpawningVolumes[i]->UpdateSpawnPoints();
			SpawningVolumes[i]->TileMapBounds = GetTileMapBounds();
			if (!SpawningVolumes[i]->GetEncounterSpawnTag().IsValid())
			{
				SpawningVolumes[i]->SetEncounterSpawnTag(LevelAreaData->EncounterSpawns[i].EncounterSpawnTag);
			}
			FString NewLabel = LevelAreaData->EncounterSpawns[i].EncounterClass->GetName().Replace(TEXT("BP"), TEXT("SpawningVolume"));
			SpawningVolumes[i]->SetActorLabel(NewLabel);
		}
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

		if (!bEnvironmentEffectsInitialized)
		{
			for (TObjectPtr<UNiagaraSystem> EnvironmentEffect : EnvironmentEffects)
            {
            	 UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, EnvironmentEffect, GetActorLocation(),
            		FRotator::ZeroRotator, FVector(1), false, false);
				SpawnedEffect->Activate();
				SpawnedEffect->SetVariableVec3(FName("ShapeLocation"), FVector{ CameraBoundary->GetScaledBoxExtent() });
            	SpawnedEnvironmentEffects.Add(SpawnedEffect);
            }
            bEnvironmentEffectsInitialized = true;
		}
		else
		{
			for (UNiagaraComponent* SpawnedEnvironmentEffect : SpawnedEnvironmentEffects)
			{
				SpawnedEnvironmentEffect->Activate();
			}
		}

	}
}

void ACameraBoundary::HandleOnBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_SetCameraInterpolation(OtherActor, this, ECameraInterpState::Entering);
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
		DestroyOutOfBoundsEncounters();
		
		// FTimerHandle DestroyTimer;
		// GetWorld()->GetTimerManager().SetTimer(DestroyTimer, FTimerDelegate::CreateLambda([this] ()
		// {
		// }), .1f, false);
		
		for (const TObjectPtr<AEncounterSpawnVolume> SpawningVolume : SpawningVolumes)
		{
			for (AEncounterSpawnPoint* SpawnPoint : SpawningVolume->GetSpawnPoints())
			{
				SpawnPoint->DespawnEncounter();
			}
		}

		ToggleLevelActorActivity(false);
		LevelArea_GrantedHandles.TakeFromAbilitySystem(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor));
		
		if (bEnvironmentEffectsInitialized)
		{
			for (UNiagaraComponent* SpawnedEnvironmentEffect : SpawnedEnvironmentEffects)
			{
				SpawnedEnvironmentEffect->DeactivateImmediate();
			}
		}
	}
}

/*
 *  Handles the case when player leaves, despawn all ?
 */
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
			// OverlapActor->Destroy();
			OverlapActor->SetLifeSpan(.5f);
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
		if (AItem* Item = Cast<AItem>(Overlap.GetActor()))
		{
			if (!bActivate && IInteractionInterface::Execute_ShouldDespawn(Item)) Overlap.GetActor()->Destroy();
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

FBoxSphereBounds ACameraBoundary::GetTileMapBounds() const
{
	return TileMap->GetRenderComponent()->Bounds;
}
