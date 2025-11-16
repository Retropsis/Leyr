// @ Retropsis 2024-2025.

#include "AI/MultiPartAnimInstance.h"
#include "AI/AICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMultiPartAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	AICharacter = Cast<AAICharacter>(GetOwningActor());
}

void UMultiPartAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!IsValid(AICharacter))
	{
		AICharacter = Cast<AAICharacter>(GetOwningActor());
	}
	if(!IsValid(AICharacter)) return;

	Velocity = AICharacter->GetCharacterMovement()->Velocity;
	bIsMoving =  UKismetMathLibrary::VSize(Velocity) > 0.f;

	const float Angle = UKismetMathLibrary::DegAcos(FVector::DotProduct(AICharacter->GetActorForwardVector().GetSafeNormal(), MovementDirection.GetSafeNormal()));
	FVector Cross = FVector::CrossProduct(AICharacter->GetActorForwardVector().GetSafeNormal(), MovementDirection.GetSafeNormal());

	// Relative Direction
	PreviousRelativeDirection = RelativeDirection;
	RelativeDirection = FMath::Sign(AICharacter->GetActorForwardVector().X) * FMath::Sign(Velocity.X) > 0 ? ERelativeDirection::Forward : ERelativeDirection::Backward;
	
	// World Direction
	bRequestTurning = false;
	PreviousDirection = Direction;
	if (Angle > -45.f && Angle < 45.f) Direction = EDirection::Right;
	if (Angle > 135.f) Direction = EDirection::Left;
	if (Angle < 135.f && Angle > 45.f)
	{
		if(Cross.Y > 0.f) Direction = EDirection::Downward;
		if(Cross.Y < 0.f) Direction = EDirection::Upward;
	}
	// GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Magenta, FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(Direction)));
	// UKismetSystemLibrary::DrawDebugArrow(this, AICharacter->GetActorLocation(), AICharacter->GetActorLocation() + MovementDirection.GetSafeNormal() * 100.f, 2.f, FLinearColor::Yellow);

	if (PreviousDirection != Direction && !bRequestTurning)
	{
		bRequestTurning = true;
	}
	
	DefeatState = AICharacter->Execute_GetDefeatState(AICharacter);
	
	// if(AICharacter->Execute_GetDefeatState(AICharacter) ==  && !DefeatState)
	// {
	// 	DefeatState = AICharacter->Execute_GetDefeatState(AICharacter);
	// 	// JumpToNode("Defeat");
	// }
}
