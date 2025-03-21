// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/SummonAbility.h"
#include "Interaction/AIInterface.h"

void USummonAbility::InitAbility()
{
	Super::InitAbility();
	
	// if (AbilityData)
	// {
	// 	NumMinions = 5;
	// 	MinionClasses;
	// 	MinSpawnDistance = 50.f;
	// 	MaxSpawnDistance = 250.f;
	// 	SpawnSpread = 90.f;
	// }
}

TArray<FVector> USummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Backward = Forward * -1.f;
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	
	const float DeltaSpread = SpawnSpread / NumMinions;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);

	float Left = Location.X - MaxSpawnDistance;
	float Right = Location.X + MaxSpawnDistance;
	if (GetAvatarActorFromActorInfo()->Implements<UAIInterface>())
	{
		FBoxSphereBounds Bounds = IAIInterface::Execute_GetNavigationBounds(GetAvatarActorFromActorInfo());
		Left = Bounds.Origin.X - Bounds.BoxExtent.X;
		Right = Bounds.Origin.X + Bounds.BoxExtent.X;
		MaxSpawnDistance = FMath::Min(FMath::Abs(Location.X - Left), FMath::Abs(Right - Location.X));
	}
	
	TArray<FVector> SpawnLocations;
	const float LinearSpread = (MaxSpawnDistance - MinSpawnDistance) / NumMinions;
	for (int32 i = 0; i < NumMinions; i++)
	{
		bool bForwardWasSuccessful = false;
		// Spawn one Forward
		FVector ChosenSpawnLocation = Location + Forward * LinearSpread * (i + 1);
		if (ChosenSpawnLocation.X > Left && ChosenSpawnLocation.X < Right)
		{
			FHitResult Hit;
			GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 100.f), ChosenSpawnLocation - FVector(0.f, 0.f, 100.f), ECC_Visibility);
			if (Hit.bBlockingHit)
			{
				ChosenSpawnLocation = Hit.ImpactPoint;
				SpawnLocations.Add(ChosenSpawnLocation);
				bForwardWasSuccessful = true;
			}
		}
		
		if (i == NumMinions - 1 && bForwardWasSuccessful) continue;
		
		// Spawn one Backward
		ChosenSpawnLocation = Location + Backward * LinearSpread * (i + 1);
		if (ChosenSpawnLocation.X > Left && ChosenSpawnLocation.X < Right)
		{
			FHitResult Hit;
			GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 100.f), ChosenSpawnLocation - FVector(0.f, 0.f, 100.f), ECC_Visibility);
			if (Hit.bBlockingHit)
			{
				ChosenSpawnLocation = Hit.ImpactPoint;
				SpawnLocations.Add(ChosenSpawnLocation);
				
				// Increment again if both successful
				if (bForwardWasSuccessful) i++;
			}
		}
	}
	return SpawnLocations;
}

TSubclassOf<APawn> USummonAbility::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}
