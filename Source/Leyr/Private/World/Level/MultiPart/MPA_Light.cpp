// @ Retropsis 2024-2025.

#include "World/Level/MultiPart/MPA_Light.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PointLightComponent.h"

AMPA_Light::AMPA_Light()
{
	LeafCapsule = CreateDefaultSubobject<UCapsuleComponent>("LeafCapsule");
	LeafCapsule->SetupAttachment(MultiPartFlipbook, "Leaf");
	LeafCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeafCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeafCapsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>("PointLightComponent");
	PointLightComponent->SetupAttachment(LeafCapsule);

	TorchComponent =  CreateDefaultSubobject<UPaperFlipbookComponent>("TorchComponent");
	TorchComponent->SetupAttachment(LeafCapsule);
	TorchComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HaloComponent =  CreateDefaultSubobject<UPaperFlipbookComponent>("HaloComponent");
	HaloComponent->SetupAttachment(LeafCapsule);
	HaloComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMPA_Light::BeginPlay()
{
	Super::BeginPlay();
	ActiveFlipbook = TorchComponent->GetFlipbook();
	MID_Halo = HaloComponent->CreateDynamicMaterialInstance(0);
	SelectFlicker();
}

void AMPA_Light::UpdateFlicker(float Alpha, float Scale)
{
	if (MID_Halo == nullptr) return;
	
	MID_Halo->SetScalarParameterValue(FName("FlickeringAlpha"), Alpha);
	MID_Halo->SetScalarParameterValue(FName("FlickeringStrength"), Alpha);
	HaloComponent->SetRelativeScale3D(FVector{ Scale, 1.f, Scale });
}

void AMPA_Light::ResetState_Implementation()
{
	Super::ResetState_Implementation();

	LeafCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeafCapsule->SetSimulatePhysics(false);
	LeafCapsule->AttachToComponent(MultiPartFlipbook, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Leaf");
	// LeafCapsule->SetRelativeLocation(FVector:: ZeroVector);
	TorchComponent->SetFlipbook(ActiveFlipbook);
	TorchComponent->SetLooping(true);
	TorchComponent->Play();
	HaloComponent->SetLooping(true);
	HaloComponent->Play();
	HaloComponent->SetVisibility(true);
	PointLightComponent->SetVisibility(true);
}
