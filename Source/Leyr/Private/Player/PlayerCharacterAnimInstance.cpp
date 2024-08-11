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
	bIsMoving =  UKismetMathLibrary::VSizeXY(Velocity) > 0.f;
	bAirborne = PlayerCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = PlayerCharacter->IsAccelerating();
}
