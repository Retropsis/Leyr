// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
class LEYR_API AArena : public AActor
{
	GENERATED_BODY()
	
public:	
	AArena();

	UFUNCTION(BlueprintPure)
	FBoundLocations GetArenaBounds() const;

	FOnPlayerOverlap OnPlayerEntering;
	FOnPlayerOverlap OnPlayerLeaving;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> EnteringArea;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> LeavingArea;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> EnteringBoundaryVisualizer;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> LeavingBoundaryVisualizer;
};
