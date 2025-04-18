// @ Retropsis 2024-2025.

#include "Player/PlayerCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"

void UPlayerCharacterAnimInstance::OnInit_Implementation()
{
	Super::OnInit_Implementation();
	PlayerCharacter = Cast<APlayerCharacter>(GetOwningActor());
}

void UPlayerCharacterAnimInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!IsValid(PlayerCharacter))
	{
		PlayerCharacter = Cast<APlayerCharacter>(GetOwningActor());
	}
	if(!IsValid(PlayerCharacter)) return;

	Velocity = PlayerCharacter->GetCharacterMovement()->Velocity;
	bIsMoving =  UKismetMathLibrary::VSize(Velocity) > 0.f;
	bIsClimbingUp = FMath::Abs(Velocity.Z) > 0.f;
	bIsClimbing = UKismetMathLibrary::VSize(Velocity) > 0.f;
	bAirborne = PlayerCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = PlayerCharacter->IsAccelerating();
	bIsCrouched = PlayerCharacter->bIsCrouched;
	CombatState = PlayerCharacter->Execute_GetCombatState(PlayerCharacter);

	if(PlayerCharacter->Execute_GetDefeatState(PlayerCharacter) == EDefeatState::Defeated && !bDead)
	{
		bDead = true;
		JumpToNode("Defeat");
	}
	if(PlayerCharacter->Execute_GetDefeatState(PlayerCharacter) == EDefeatState::Executed && !bExecuted)
	{
		bExecuted = true;
		JumpToNode("Executed");
	}

	GEngine->AddOnScreenDebugMessage(159, 1.f, FColor::Cyan, FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(CombatState)));
}

void UPlayerCharacterAnimInstance::PlayVaultDownSequence()
{
	PlayAnimationOverride(VaultDownSequence);
}
