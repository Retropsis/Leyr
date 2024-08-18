// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AI/BaseCharacter.h"
#include "Interaction/InventoryInterface.h"
#include "Interaction/PlayerInterface.h"
#include "PlayerCharacter.generated.h"

class UPlayerInventoryComponent;
class UCameraComponent;
class USpringArmComponent;

/**
 * 
 */
UCLASS()
class LEYR_API APlayerCharacter : public ABaseCharacter, public IPlayerInterface, public IInventoryInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	void Move(float ScaleValue);
	void RotateController() const;
	
	/** Combat Interface */
	virtual int32 GetCharacterLevel() override;
	/** end Combat Interface */
	
	/** Inventory Interface */
	virtual UInventoryComponent* GetInventoryComponent_Implementation() override { return InventoryComponent; }
	/** end Inventory Interface */

protected:
	virtual void InitAbilityActorInfo() override;
	
	bool bAirborne = false;
	bool bIsAccelerating = false;
	bool bIsMoving = false;

private:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInventoryComponent> InventoryComponent;

public:
	FORCEINLINE bool IsAirborne() const { return bAirborne; }
	FORCEINLINE bool IsAccelerating() const { return bIsAccelerating; }
	FORCEINLINE bool IsMoving() const { return bIsMoving; }
};
