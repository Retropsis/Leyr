// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointCollection.generated.h"

UCLASS()
class LEYR_API APointCollection : public AActor
{
	GENERATED_BODY()
	
public:	
	APointCollection();
 
	UFUNCTION(BlueprintPure)
	TArray<USceneComponent*> GetGroundPoints(int32 NumPoints, float YawOverride = 0.f);
 	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float LiveActorRadius = 350.f;
	
	UPROPERTY(EditDefaultsOnly)
	float RaisedLocationZ = 50.f;
	
	UPROPERTY(EditDefaultsOnly)
	float LoweredLocationZ = 75.f;
 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<TObjectPtr<USceneComponent>> Points;
 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> CollectionRoot;
 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_1;
 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_2;
 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_3;
 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_4;
 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_5;
 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_6;
 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_7;
 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_8;
 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_9;
 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_10;

};
