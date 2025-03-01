// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World/Data/MapInfo.h"
#include "ParallaxController.generated.h"

class UCameraComponent;
class UPaperSpriteComponent;

UCLASS()
class LEYR_API AParallaxController : public AActor
{
	GENERATED_BODY()
	
public:	
	AParallaxController();
	virtual void Tick(float DeltaSeconds) override;
	void InitializeMapParallax(AActor* ActorToAttach);

	UPROPERTY(EditAnywhere)
	FName CurrentMapName = FName();
	
	UPROPERTY()
	TObjectPtr<UCameraComponent> Camera;

protected:
	UPROPERTY()
	TObjectPtr<UPaperSpriteComponent> Sky;

	UPROPERTY()
	TObjectPtr<UPaperSpriteComponent> Sun;
	
	UPROPERTY()
	TObjectPtr<UPaperSpriteComponent> FirstMoon;
	
	UPROPERTY()
	TObjectPtr<UPaperSpriteComponent> SecondMoon;
	
	UPROPERTY()
	TObjectPtr<UPaperSpriteComponent> BackClouds;
	
	UPROPERTY()
	TObjectPtr<UPaperSpriteComponent> FrontClouds;
	
	UPROPERTY()
	TObjectPtr<UPaperSpriteComponent> BackLayer;
	
	UPROPERTY()
	TObjectPtr<UPaperSpriteComponent> FrontLayer;
	
	UPROPERTY()
	TObjectPtr<UPaperSpriteComponent> ForegroundFogLayer;
	
	UPROPERTY() 
	TObjectPtr<UPaperSpriteComponent> BackgroundFogLayer;
	
	UPROPERTY() 
	TObjectPtr<UPaperSpriteComponent> ForegroundLayer;
	
	bool bSunIsVisible = true;
	bool bFirstMoonIsVisible = false;
	bool bSecondMoonIsVisible = false;
	bool bBackCloudsAreVisible = false;
	bool bFrontCloudsAreVisible = false;
	bool bBackLayerIsVisible = false;
	bool bFrontLayerIsVisible = false;
	bool bForegroundFogIsVisible = false;
	bool bBackgroundFogIsVisible = false;
	bool bForegroundLayerIsVisible = false;
	
	FVector2D SunSpeed = FVector2D::ZeroVector;
	FVector2D FirstMoonSpeed = FVector2D::ZeroVector;
	FVector2D SecondMoonSpeed = FVector2D::ZeroVector;
	FVector2D BackCloudsSpeed = FVector2D::ZeroVector;
	FVector2D FrontCloudsSpeed = FVector2D::ZeroVector;
	FVector2D BackLayerSpeed = FVector2D::ZeroVector;
	FVector2D FrontLayerSpeed = FVector2D::ZeroVector;
	FVector2D ForegroundFogLayerSpeed = FVector2D::ZeroVector;
	FVector2D BackgroundFogLayerSpeed = FVector2D::ZeroVector;
	FVector2D ForegroundLayerSpeed = FVector2D::ZeroVector;

private:
	UPROPERTY()
	TMap<UMaterialInstanceDynamic*, FParallaxLayerData> DynamicLayersToSpeed;
	
	float DeltaSecondsNormalized = 0.f;
};
