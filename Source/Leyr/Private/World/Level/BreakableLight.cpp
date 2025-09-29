// @ Retropsis 2024-2025.

#include "World/Level/BreakableLight.h"
#include "NiagaraFunctionLibrary.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"

ABreakableLight::ABreakableLight()
{
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>("PointLightComponent");
	PointLightComponent->SetupAttachment(BoxComponent);

	HaloComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("HaloComponent");
	HaloComponent->SetupAttachment(BoxComponent);
	HaloComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HaloComponent->SetRelativeLocation(FVector(0, -5.f, 0));

	GetRenderComponent()->SetupAttachment(BoxComponent);
}

void ABreakableLight::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
	ActiveFlipbook = GetRenderComponent()->GetFlipbook();
	BoxComponent->OnComponentHit.AddUniqueDynamic(this, &ABreakableLight::OnHit);
	MID_Halo = HaloComponent->CreateDynamicMaterialInstance(0);
	SelectFlicker();
}

void ABreakableLight::UpdateFlicker(float Alpha, float Scale)
{
	if (MID_Halo == nullptr) return;
	
	MID_Halo->SetScalarParameterValue(FName("FlickeringAlpha"), Alpha);
	MID_Halo->SetScalarParameterValue(FName("FlickeringStrength"), Alpha);
	HaloComponent->SetRelativeScale3D(FVector{ Scale, 1.f, Scale });
}

void ABreakableLight::Extinguish()
{
	BoxComponent->OnComponentHit.RemoveAll(this);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	BoxComponent->SetSimulatePhysics(true);
	GetRenderComponent()->SetFlipbook(ExtinguishFlipbook);
	GetRenderComponent()->SetLooping(false);
	PointLightComponent->SetVisibility(false);
	HaloComponent->SetVisibility(false);
	bHasInteracted = true;

	FTimerHandle ResetCollisionTimer;
	GetWorld()->GetTimerManager().SetTimer(ResetCollisionTimer, FTimerDelegate::CreateLambda([this] ()
	{
		BoxComponent->SetSimulatePhysics(false);
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}), 2.f, false);
}

void ABreakableLight::HandleOnHit(const FHitResult& HitResult)
{
	if (!bHit)
	{
		bHit = true;
		BoxComponent->OnComponentHit.RemoveAll(this);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, HitResult.Location + FVector{ 0.f, 20.f, 20.f });
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, HitResult.Location);
	}
}

void ABreakableLight::ResetState_Implementation()
{
	BoxComponent->OnComponentHit.AddUniqueDynamic(this, &ABreakableLight::OnHit);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
	// BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetSimulatePhysics(false);
	BoxComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	BoxComponent->SetRelativeLocation(FVector:: ZeroVector);
	BoxComponent->SetRelativeRotation(FRotator::ZeroRotator);
	// BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	bHasInteracted = false;
	HaloComponent->SetVisibility(true);
	PointLightComponent->SetVisibility(true);
	
	GetRenderComponent()->SetFlipbook(ActiveFlipbook);
	GetRenderComponent()->Play();
	GetRenderComponent()->SetLooping(true);
}
