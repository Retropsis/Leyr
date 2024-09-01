// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Platform.generated.h"

class UBoxComponent;

UENUM(BlueprintType)
enum class EPlatformType : uint8
{
	Default,
	Moving,
	Cycling,
	Weighting,
	Transient,
};

UCLASS()
class LEYR_API APlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	APlatform();

protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Platform")
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UPROPERTY(EditDefaultsOnly, Category="Platform")
	TObjectPtr<UBoxComponent> OverlapBox;

	UPROPERTY(EditDefaultsOnly)
	EPlatformType Platform = EPlatformType::Default;
};
