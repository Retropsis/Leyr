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

	HaloComponent =  CreateDefaultSubobject<UPaperFlipbookComponent>("HaloComponent");
	HaloComponent->SetupAttachment(BoxComponent);
	HaloComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetRenderComponent()->SetupAttachment(BoxComponent);
}

void ABreakableLight::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
	ActiveFlipbook = GetRenderComponent()->GetFlipbook();
	BoxComponent->OnComponentHit.AddUniqueDynamic(this, &ABreakableLight::OnHit);
}

void ABreakableLight::Extinguish()
{
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	BoxComponent->SetSimulatePhysics(true);
	GetRenderComponent()->SetFlipbook(ExtinguishFlipbook);
	GetRenderComponent()->SetLooping(false);
	BoxComponent->OnComponentHit.RemoveAll(this);
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
	BoxComponent->SetSimulatePhysics(false);
	BoxComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	BoxComponent->SetRelativeLocation(FVector:: ZeroVector);
	
	HaloComponent->SetVisibility(true);
	
	GetRenderComponent()->SetFlipbook(ActiveFlipbook);
	GetRenderComponent()->Play();
	GetRenderComponent()->SetLooping(true);
}
