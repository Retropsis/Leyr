// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Data/CombatData.h"
#include "GameFramework/Actor.h"
#include "OverlapZone.generated.h"

enum class ECombatState : uint8;
class UBoxComponent;

UCLASS()
class LEYR_API AOverlapZone : public AActor
{
	GENERATED_BODY()
	
public:	
	AOverlapZone();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> OverlapZone;

	UPROPERTY(EditAnywhere)
	ECombatState RequiredState = ECombatState::WalkingPeaceful;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBillboardComponent> LevelDesignIcon;
};
