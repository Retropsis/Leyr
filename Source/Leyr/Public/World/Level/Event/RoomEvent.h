// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Event.h"
#include "RoomEvent.generated.h"

UCLASS()
class LEYR_API ARoomEvent : public AEvent
{
	GENERATED_BODY()

public:
	ARoomEvent();

	//~ Save Interface
	virtual void LoadActor_Implementation() override;
	//~ Save Interface

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void HandleEventOwnerDefeated(AActor* DefeatedEncounter);

private:
	UPROPERTY(EditAnywhere, Category="Event")
	TObjectPtr<AActor> EventOwner;
};
