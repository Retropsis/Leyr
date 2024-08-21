// @ Retropsis 2024-2025.

#include "AbilitySystem/Ability/SummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> USummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	
	const float DeltaSpread = SpawnSpread / NumMinions;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	
	TArray<FVector> SpawnLocations;
	// for (int32 i = 0; i < NumMinions; i++)
	// {
	// 	const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
	// 	const FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
	// 	SpawnLocations.Add(ChosenSpawnLocation);
	//
	// 	DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 18.f, 12, FColor::Cyan, false, 3.f );
	// 	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + Direction * MaxSpawnDistance, 4.f, FLinearColor::Green, 3.f );
	// 	DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 5.f, 12, FColor::Red, false, 3.f );
	// 	DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 5.f, 12, FColor::Red, false, 3.f );
	// }

	//TODO: Add spawns behind summoner as option
	const float LinearSpread = (MaxSpawnDistance - MinSpawnDistance) / (NumMinions - 1);
	// DrawDebugSphere(GetWorld(), Location + Forward * MinSpawnDistance, 5.f, 12, FColor::Red, false, 3.f );
	// DrawDebugSphere(GetWorld(), Location + Forward * MaxSpawnDistance, 5.f, 12, FColor::Red, false, 3.f );
	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location + Forward * MinSpawnDistance, Location + Forward * MaxSpawnDistance, 4.f, FLinearColor::Green, 3.f );
	for (int32 i = 0; i < NumMinions; i++)
	{
		FVector ChosenSpawnLocation = Location + Forward * LinearSpread * (i + 1);
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 100.f), ChosenSpawnLocation - FVector(0.f, 0.f, 100.f), ECC_Visibility);
		
		if (Hit.bBlockingHit) ChosenSpawnLocation = Hit.ImpactPoint;
		
		SpawnLocations.Add(ChosenSpawnLocation);
	}
	return SpawnLocations;
}

TSubclassOf<APawn> USummonAbility::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}
