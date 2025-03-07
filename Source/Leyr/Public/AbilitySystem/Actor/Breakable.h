// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilityActor.h"
#include "PaperFlipbookActor.h"
#include "Interaction/ConstructionInterface.h"
#include "UI/Controller/OverlayWidgetController.h"
#include "Breakable.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class LEYR_API ABreakable : public AAbilityActor, public IConstructionInterface
{
	GENERATED_BODY()

public:
	ABreakable();

	//~ Construction Interface
	virtual void ShowTopSprite_Implementation(bool bShow) override;
	//~ Construction Interface
	
protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void DestroyActor_Implementation() override;
	
	virtual void MulticastHandleDestruction_Implementation() override;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Platform")
	TObjectPtr<UBoxComponent> HitBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Platform")
	TObjectPtr<UPaperFlipbookComponent> FlipbookComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Platform")
	TObjectPtr<UPaperFlipbookComponent> TopFlipbookComponent;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(EditDefaultsOnly, Category="Breakable")
	int32 MaxHealthyFrame = 2;
};
