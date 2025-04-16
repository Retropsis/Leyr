// @ Retropsis 2024-2025.

#include "AI/BaseAIController.h"
#include "AI/AICharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

ABaseAIController::ABaseAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComponent);
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ABaseAIController::HandlePerceptionUpdated);
}

void ABaseAIController::HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		if(Actor->ActorHasTag("Player"))
		{
			FActorPerceptionBlueprintInfo Info;
			PerceptionComponent->GetActorsPerception(Actor, Info);
			for (const FAIStimulus AIStimulus : Info.LastSensedStimuli)
			{
				AAICharacter* ControlledCharacter = Cast<AAICharacter>(GetPawn());
				EBehaviourState State = ControlledCharacter->GetBehaviourState();
				if(IEnemyInterface::Execute_ShouldAttack(GetPawn()) && (State == EBehaviourState::Patrol || State == EBehaviourState::Search))
				{
					UseSightSense(AIStimulus);
				}
			}
		}
	}
}

void ABaseAIController::UseSightSense(const FAIStimulus& ActiveStimulus) const
{
	if(ActiveStimulus.WasSuccessfullySensed())
	{
		// GEngine->AddOnScreenDebugMessage(9877, 3.5f, FColor::Green, FString::Printf(TEXT("Player On Sight !!")));
		if(AAICharacter* ControlledCharacter = Cast<AAICharacter>(GetPawn()))
		{
			ControlledCharacter->HandleBehaviourState(EBehaviourState::Chase);
		}
	}
	else
	{
		// GEngine->AddOnScreenDebugMessage(9878, 3.5f, FColor::Red, FString::Printf(TEXT("Player NOT On Sight !!")));
	}
}
