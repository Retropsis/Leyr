// @ Retropsis 2024-2025.

#include "AI/AICharacterAnimInstance.h"
#include "AI/AICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UAICharacterAnimInstance::OnInit_Implementation()
{
	Super::OnInit_Implementation();
	AICharacter = Cast<AAICharacter>(GetOwningActor());
}

void UAICharacterAnimInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!IsValid(AICharacter))
	{
		AICharacter = Cast<AAICharacter>(GetOwningActor());
	}
	if(!IsValid(AICharacter)) return;

	Velocity = AICharacter->GetCharacterMovement()->Velocity;
	bIsMoving =  UKismetMathLibrary::VSize(Velocity) > 0.f || AICharacter->GetBehaviourState() == EBehaviourState::Timeline;
	bAirborne = AICharacter->GetCharacterMovement()->IsFalling();
	// bIsAccelerating = AICharacter->IsAccelerating();
	bIsDiving = AICharacter->GetBehaviourState() == EBehaviourState::Dive;
	
	DefeatState = AICharacter->Execute_GetDefeatState(AICharacter);
	
	// if(AICharacter->Execute_GetDefeatState(AICharacter) ==  && !DefeatState)
	// {
	// 	DefeatState = AICharacter->Execute_GetDefeatState(AICharacter);
	// 	// JumpToNode("Defeat");
	// }
}
