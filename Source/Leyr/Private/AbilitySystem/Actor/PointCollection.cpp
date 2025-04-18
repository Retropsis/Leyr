// @ Retropsis 2024-2025.

#include "AbilitySystem/Actor/PointCollection.h"
#include "AbilitySystem/LeyrAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#define CREATE_AND_SETUP_PT(Pt_X) \
Pt_X = CreateDefaultSubobject<USceneComponent>(TEXT(#Pt_X)); \
Points.Add(Pt_X); \
Pt_X->SetupAttachment(GetRootComponent());

APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;
 
	CollectionRoot = CreateDefaultSubobject<USceneComponent>("CollectionRoot");
	SetRootComponent(CollectionRoot);
	
	CREATE_AND_SETUP_PT(Pt_1);
	CREATE_AND_SETUP_PT(Pt_2);
	CREATE_AND_SETUP_PT(Pt_3);
	CREATE_AND_SETUP_PT(Pt_4);
	CREATE_AND_SETUP_PT(Pt_5);
	CREATE_AND_SETUP_PT(Pt_6);
	CREATE_AND_SETUP_PT(Pt_7);
	CREATE_AND_SETUP_PT(Pt_8);
	CREATE_AND_SETUP_PT(Pt_9);
	CREATE_AND_SETUP_PT(Pt_10);
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(int32 NumPoints, float YawOverride)
{
	if (NumPoints > Points.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to access Points out of bounds. It was set to Points.Num instead"));
		NumPoints = Points.Num();
	}
 
	TArray<USceneComponent*> ArrayCopy;
 
	for (const TObjectPtr<USceneComponent>& Point : Points)
	{ 
		FVector ToPoint = Point->GetComponentLocation() - GetActorLocation();
		ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
		Point->SetWorldLocation(GetActorLocation() + ToPoint);
 
		const FVector RaisedLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, Point->GetComponentLocation().Z + RaisedLocationZ);
		const FVector LoweredLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, Point->GetComponentLocation().Z - LoweredLocationZ);
 
		FHitResult HitResult;
		TArray<AActor*> IgnoreActors;
		ULeyrAbilitySystemLibrary::GetLivePlayersWithinRadius(this, IgnoreActors, TArray<AActor*>(), LiveActorRadius, GetActorLocation());
 
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		UKismetSystemLibrary::LineTraceSingle(this, RaisedLocation, LoweredLocation, TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true);

		if (HitResult.bBlockingHit)
		{
			const FVector AdjustedLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
			Point->SetWorldLocation(AdjustedLocation);
			Point->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));
 
			ArrayCopy.Add(Point);
			if (ArrayCopy.Num() == NumPoints) break;
		}
	}
	return ArrayCopy;
}

TArray<FVector> APointCollection::GetGroundLocations(int32 NumPoints)
{
	if (NumPoints > Points.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to access Points out of bounds. It was set to Points.Num instead"));
		NumPoints = Points.Num();
	}
 
	TArray<FVector> ArrayCopy;
 
	for (const TObjectPtr<USceneComponent>& Point : Points)
	{ 
		FVector ToPoint = Point->GetComponentLocation() - GetActorLocation();
		Point->SetWorldLocation(GetActorLocation() + ToPoint);
 
		const FVector RaisedLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, Point->GetComponentLocation().Z + RaisedLocationZ);
		const FVector LoweredLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, Point->GetComponentLocation().Z - LoweredLocationZ);
 
		FHitResult HitResult;
		TArray<AActor*> IgnoreActors;
		ULeyrAbilitySystemLibrary::GetLivePlayersWithinRadius(this, IgnoreActors, TArray<AActor*>(), LiveActorRadius, GetActorLocation());
 
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		UKismetSystemLibrary::LineTraceSingle(this, RaisedLocation, LoweredLocation, TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true);

		if (HitResult.bBlockingHit)
		{
			const FVector AdjustedLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
			Point->SetWorldLocation(AdjustedLocation);
			Point->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));
 
			ArrayCopy.Add(Point->GetComponentLocation());
			if (ArrayCopy.Num() == NumPoints) break;
		}
	}
	return ArrayCopy;
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
}

