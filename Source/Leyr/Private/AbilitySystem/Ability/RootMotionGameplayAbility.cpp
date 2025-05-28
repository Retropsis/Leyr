// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/RootMotionGameplayAbility.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"

void URootMotionGameplayAbility::InitAbility()
{
	if (FetchAnimInstances()) return;
	
	Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	
	ToggleCapsuleCollision(false);
	DeterminePoise();
}

void URootMotionGameplayAbility::PrepareToEndAbility()
{
	ToggleCapsuleCollision(true);
	ICombatInterface::Execute_SetCombatStateToHandle(GetAvatarActorFromActorInfo(), ECombatState::Unoccupied, FCombatStateParams{});
	RemovePoise();
}

void URootMotionGameplayAbility::ToggleCapsuleCollision(const bool bEnable) const
{
	if (Character)
	{
		Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, bEnable ? ECR_Block : ECR_Ignore);
	}
}
