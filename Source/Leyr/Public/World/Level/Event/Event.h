// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "Interaction/InteractionInterface.h"
#include "Interaction/SaveInterface.h"
#include "Event.generated.h"

UENUM(BlueprintType)
enum class EEventState : uint8
{
	None,
	On,
	Off,
	Timer,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEventStateChanged, EEventState NewState);

UCLASS()
class LEYR_API AEvent : public APaperFlipbookActor, public IInteractionInterface, public ISaveInterface
{
	GENERATED_BODY()

public:
	AEvent();
	EEventState GetEventState() const { return EventState; }

	UFUNCTION(BlueprintCallable)
	void SetEventState(const EEventState NewState);
	
	FOnEventStateChanged OnEventStateChanged;

protected:
	UPROPERTY(EditAnywhere, SaveGame)
	EEventState EventState = EEventState::Off;
};
