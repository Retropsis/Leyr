// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilityActor.h"
#include "PaperFlipbookActor.h"
#include "Data/MapData.h"
#include "Interaction/ConstructionInterface.h"
#include "Interaction/SaveInterface.h"
#include "UI/Controller/OverlayWidgetController.h"
#include "Breakable.generated.h"

class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHiddenrevealed, const FIntPoint&, SubdivisionCoords, ESubdivisionSide, Side);

UCLASS()
class LEYR_API ABreakable : public AAbilityActor, public IConstructionInterface, public ISaveInterface
{
	GENERATED_BODY()

public:
	ABreakable();

	//~ Construction Interface
	virtual void ShowTopSprite_Implementation(bool bShow) override;
	//~ Construction Interface
	
	//~ AbilityActor Interface
	virtual void DestroyActor_Implementation() override;
	//~ AbilityActor Interface
	
protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void MulticastHandleDestruction_Implementation() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breakable")
	TObjectPtr<UBoxComponent> HitBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breakable")
	TObjectPtr<UPaperFlipbookComponent> FlipbookComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breakable")
	TObjectPtr<UPaperFlipbookComponent> TopFlipbookComponent;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(EditDefaultsOnly, Category="Breakable")
	int32 MaxHealthyFrame = 2;

	UPROPERTY(EditDefaultsOnly, Category="Breakable")
	bool bIsHiddenWall = false;

	UPROPERTY(SaveGame)
	bool bWasRevealed = false;

	ESubdivisionSide SubdivisionSide = ESubdivisionSide::None;
	FIntPoint SubdivisionCoordinates;
	FOnHiddenrevealed OnHiddenRevealed;
};
