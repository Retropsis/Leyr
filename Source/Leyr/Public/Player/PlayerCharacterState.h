// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "PlayerCharacterState.generated.h"

class ULevelUpInfo;
class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /* StatValue */)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 /* StatValue */, bool /* bLevelUp */)

/**
 * 
 */
UCLASS()
class LEYR_API APlayerCharacterState : public APlayerState,  public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APlayerCharacterState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	FORCEINLINE int32 GetCharacterLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSkillPoints() const { return SkillPoints; }

	void AddToXP(int32 InXP);
	void AddToLevel(int32 InLevel);

	void SetXP(int32 InXP);
	void SetLevel(int32 InLevel);
	void SetAttributePoints(int32 InPoints);
	void SetSkillPoints(int32 InPoints);
	
	void AddToAttributePoints(int32 InPoints);
	void AddToSkillPoints(int32 InPoints);
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
	FOnPlayerStatChanged OnXPChangedDelegate;
	FOnLevelChanged OnLevelChangedDelegate;
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	FOnPlayerStatChanged OnSkillPointsChangedDelegate;

protected:
	/*
	 * Ability System
	 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 1;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SkillPoints)
	int32 SkillPoints = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
	UFUNCTION()
	void OnRep_XP(int32 OldXP);
	
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

	UFUNCTION()
	void OnRep_SkillPoints(int32 OldSkillPoints);
};
