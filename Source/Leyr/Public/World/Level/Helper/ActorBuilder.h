// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World/Level/LevelDesignData.h"
#include "ActorBuilder.generated.h"

UCLASS()
class LEYR_API AActorBuilder : public AActor
{
	GENERATED_BODY()
	
public:	
	AActorBuilder();
	void ClearChildActors();

	UFUNCTION(CallInEditor)
	void Build();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ActorBuilder")
	int32 WidthCount = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ActorBuilder")
	int32 HeightCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ActorBuilder")
	float TileWidth = 64.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ActorBuilder")
	float TileHeight = 64.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ActorBuilder")
	TSubclassOf<AActor> ChildActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ActorBuilder")
	bool bShowTopSprite = true;

private:
	UPROPERTY()
	TArray<AActor*> ChildActors;
};
