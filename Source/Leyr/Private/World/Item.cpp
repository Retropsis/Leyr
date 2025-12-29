// @ Retropsis 2024-2025.

#include "World/Item.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "PaperFlipbookComponent.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Data/ItemDataRow.h"
#include "Interaction/InventoryInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/Component/MapComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	// Sphere->SetupAttachment(GetRootComponent());
	Sphere->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
	GetRenderComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetRenderComponent()->SetupAttachment(GetRootComponent());
}

void AItem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	RunningTime += DeltaSeconds;
	const float SinePeriod = 2 * PI / SinePeriodConstant;
	if (RunningTime > SinePeriod)
	{
		RunningTime = 0.f;
	}
	ItemMovement(DeltaSeconds);
}

#if WITH_EDITOR
void AItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if(ChangedPropertyName == GET_MEMBER_NAME_CHECKED(FDataTableRowHandle, RowName))
	{
		InitializeItemFromDataTable(ItemRowHandle.RowName);
	}
}
#endif

void AItem::InitializeItemFromDataTable(const FName& RowName, const bool bDespawn)
{
	if(ItemDataTable)
	{
		if(const FItemDataRow* ItemDataRow = ItemDataTable->FindRow<FItemDataRow>(RowName, RowName.ToString()))
		{
			if(const UItemData* Asset = ItemDataRow->ItemData.Asset.LoadSynchronous())
			{
				PickupSound = Asset->PickupSound;
				GetRenderComponent()->SetFlipbook(Asset->PickupFlipbook);
				ItemData.bStackable = Asset->bIsStackable;
				ItemData.EquipmentSlot = Asset->EquipmentSlot;
				if (Asset->ItemType == EItemType::ActivateOnPickup) Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnBeginOverlap);
			}
			ItemData = ItemDataRow->ItemData;
			bShouldDespawn = bDespawn;
#if WITH_EDITOR
			SetActorLabel(FString::Printf(TEXT("BP_%s"), *ItemData.Asset.GetAssetName()));
#endif
		}
	}
}

void AItem::LoadActor_Implementation()
{
	if (bPickedUp)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetRenderComponent()->SetVisibility(false);
		bPickedUp = true;
	}
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
	CalculatedRotation = GetActorRotation();
	
	if(HasAuthority())
	{
		if(!ItemRowHandle.RowName.IsNone())
		{
			ItemData = ULeyrAbilitySystemLibrary::FindItemDataByRowName(this, ItemRowHandle.RowName);
		}
		Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Sphere->SetNotifyRigidBodyCollision(true);
		Sphere->OnComponentHit.AddDynamic(this, &AItem::HandleOnHit);
	
		if(ItemData.ID == 0)
		{
			
		}
	}
}

void AItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if (UInventoryComponent* InventoryComponent = IInventoryInterface::Execute_GetInventoryComponent(OtherActor))
	// {
	// 	// if (InventoryComponent->TryAddItem(ItemData)) Destroy();
	// }
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		for (const TSubclassOf<UGameplayEffect> Effect : ItemData.Asset.Get()->Effects)
		{
			if (IsValid(Effect))
			{
				const UGameplayEffect* GameplayEffect = Effect->GetDefaultObject<UGameplayEffect>();
				FGameplayEffectContextHandle ContextHandle =TargetASC->MakeEffectContext();
				ContextHandle.AddSourceObject(TargetASC->GetAvatarActor());
				TargetASC->ApplyGameplayEffectToSelf(GameplayEffect, 1.f, ContextHandle);
			}
		}
		Destroy();
	}
}

void AItem::HandleOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Sphere->SetSimulatePhysics(false);
	Sphere->SetEnableGravity(false);
	Sphere->SetNotifyRigidBodyCollision(false);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bSinusoidalMovement = true;
	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
}

void AItem::Interact_Implementation(AActor* InteractingActor)
{	
	if (UInventoryComponent* InventoryComponent = IInventoryInterface::Execute_GetInventoryComponent(InteractingActor))
	{
		InventoryComponent->ServerAddItem(ItemData);
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetRenderComponent()->SetVisibility(false);
		bPickedUp = true;
	}
	if (const APlayerController* PlayerController = Cast<APlayerController>(InteractingActor->GetInstigatorController()))
	{
		if (UMapComponent* MapComponent = PlayerController->FindComponentByClass<UMapComponent>())
		{
			MapComponent->UnveilRooms(RoomsToUnveil);
		}
	}
}

void AItem::StartSinusoidalMovement()
{
	Sphere->SetSimulatePhysics(true);
	Sphere->SetEnableGravity(true);
	// bSinusoidalMovement = true;
}

void AItem::StartRotation()
{
	bRotates = true;
	CalculatedRotation = GetActorRotation();
}

void AItem::ItemMovement(float DeltaTime)
{
	if (bRotates)
	{
		const FRotator DeltaRotation(0.f, DeltaTime * RotationRate, 0.f);
		CalculatedRotation = UKismetMathLibrary::ComposeRotators(CalculatedRotation, DeltaRotation);
	}
	if (bSinusoidalMovement)
	{
		const float Sine = SineAmplitude * FMath::Sin(RunningTime * SinePeriodConstant);
		CalculatedLocation = InitialLocation + FVector(0.f, 0.f, Sine);
	}
}