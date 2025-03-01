// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World/Map/CameraBoundary.h"
#include "Arena.generated.h"

USTRUCT(BlueprintType)
struct FBoundLocations
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FVector Left = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly)
	FVector Right = FVector::ZeroVector;
};

class UBoxComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerOverlap, AActor* Player);

UCLASS()
class LEYR_API AArena : public ACameraBoundary
{
	GENERATED_BODY()
	
public:	
	AArena();

	UFUNCTION(BlueprintPure)
	FBoundLocations GetArenaBounds() const;

	FOnPlayerOverlap OnPlayerEntering;
	FOnPlayerOverlap OnPlayerLeaving;
	
	virtual void InitializeCameraExtent() override;
	virtual void HandleOnBeginOverlap(AActor* OtherActor) override;

	UFUNCTION()
	void HandleOnOwnerDeath(AActor* DeadActor);

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnLeavingBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> LeavingBoundary;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> ArenaBoundary;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> LeavingBoundaryVisualizer;
};
