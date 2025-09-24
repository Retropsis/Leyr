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
	// DefeatState = ICombatInterface::Execute_GetDefeatState(AICharacter);
	// if(AICharacter->Implements<UCombatInterface>())
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(AICharacter->Execute_GetDefeatState(AICharacter))));
	// }
	
	if(DefeatState == EDefeatState::Defeated)
	{
		JumpToNode("Destroyed");
	}
}
