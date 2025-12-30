// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Item/Item.h"
#include "AttributeUpgrade.generated.h"

UCLASS()
class LEYR_API AAttributeUpgrade : public AItem
{
	GENERATED_BODY()

public:
	AAttributeUpgrade();
	//~ Interaction Interface
	virtual void Interact_Implementation(AActor* InteractingActor) override;
	//~ Interaction Interface

private:
	UPROPERTY(EditAnywhere, Category="Leyr", meta=(Categories="Attributes"))
	FGameplayTag	AttributeTag = FGameplayTag();
};
