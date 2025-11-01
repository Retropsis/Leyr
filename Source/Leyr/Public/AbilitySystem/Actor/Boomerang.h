// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Actor/Projectile.h"
#include "Boomerang.generated.h"

class URotatingMovementComponent;
/**
 * 
 */
UCLASS()
class LEYR_API ABoomerang : public AProjectile
{
	GENERATED_BODY()

public:
	ABoomerang();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TravelDistance = 350.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InstigatorDistance = 25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RotationRate = 75.f;
	
	UPROPERTY(BlueprintReadWrite)
	FVector InitialLocation = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite)
	FVector ApexLocation = FVector::ZeroVector;
	
	FVector InitialDirection = FVector::ZeroVector;

	UFUNCTION()
	void OnActivated(UActorComponent* Component, bool bReset);
	
protected:
	virtual void BeginPlay() override;
	virtual void OnHit() override;
	virtual void OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	// virtual bool IsValidOverlap(const AActor* OtherActor) override;
	
	UFUNCTION(BlueprintCallable)
	void OutgoingTimelineEnd();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URotatingMovementComponent> RotatingMovementComponent;
};
