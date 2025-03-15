// @ Retropsis 2024-2025.

#include "AsyncTasks/KeepDistanceTask.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"

UKeepDistanceTask* UKeepDistanceTask::KeepDistance(AAIController* Controller, APawn* ControlledPawn, FVector TargetLocation, AActor* TargetActor, float AcceptanceRadius, bool bBackward)
{
	UKeepDistanceTask* Task = Controller ? NewAITask<UKeepDistanceTask>(*Controller) : nullptr;
	if (Task)
	{
		Task->Controller = Controller;
		Task->ControlledPawn = ControlledPawn;
		Task->TargetActor = TargetActor;
		Task->TargetLocation = TargetLocation;
		Task->AcceptanceRadius = AcceptanceRadius;
		Task->bBackward = bBackward;
	}
	return Task;
}

void UKeepDistanceTask::Activate()
{
	Super::Activate();
	TargetLocation = TargetActor->GetActorLocation();
		
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ControlledPawn->GetActorLocation(), FVector(TargetLocation.X, ControlledPawn->GetActorLocation().Y, TargetLocation.Z));
	const FRotator WorldDirection = FRotator(LookAtRotation.Pitch, 0.f, 0.f);
	ControlledPawn->AddMovementInput(LookAtRotation.Vector(), 1.f, true);
		
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Moving To");
		
	if(!bBackward && FVector::DotProduct(LookAtRotation.Vector(), ControlledPawn->GetActorForwardVector()) < 0.f)
	{
		ControlledPawn->SetActorRotation(UKismetMathLibrary::ComposeRotators(ControlledPawn->GetActorRotation(), FRotator(0.f, 180.f, 0.f)));
	}
	if (TargetActor && ControlledPawn->GetDistanceTo(TargetActor) <= AcceptanceRadius)
	{
		OnMoveComplete.Broadcast();
	} else OnMoveFailed.Broadcast();
}
