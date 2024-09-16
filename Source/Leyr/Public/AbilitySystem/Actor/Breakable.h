// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilityActor.h"
#include "PaperFlipbookActor.h"
#include "UI/Controller/OverlayWidgetController.h"
#include "Breakable.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class LEYR_API ABreakable : public AAbilityActor
{
	GENERATED_BODY()

public:
	ABreakable();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void DestroyActor_Implementation() override;
	
	virtual void MulticastHandleDestruction_Implementation() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Platform")
	TObjectPtr<UBoxComponent> HitBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Platform")
	TObjectPtr<UPaperFlipbookComponent> FlipbookComponent;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(EditDefaultsOnly, Category="Breakable")
	int32 MaxHealthyFrame = 2;
};
