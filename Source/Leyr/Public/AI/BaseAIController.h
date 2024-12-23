// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BaseAIController.generated.h"

class AAICharacter;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class LEYR_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABaseAIController();
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	
protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	void UseSightSense(const FAIStimulus& ActiveStimulus) const;
};
