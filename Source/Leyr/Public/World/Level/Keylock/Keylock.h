// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "World/Level/Lever.h"
#include "Keylock.generated.h"

class UItemData;
/**
 * 
 */
UCLASS()
class LEYR_API AKeylock : public ALever
{
	GENERATED_BODY()
	
public:	
	AKeylock();

	//~ Save Interface
	virtual void LoadActor_Implementation() override;
	//~ Save Interface

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UItemData> KeyData;

protected:
	virtual void BeginPlay() override;
	virtual void Interact_Implementation(AActor* InteractingActor) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> LockHitBox;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPaperFlipbookComponent> LockFlipbook;
};
