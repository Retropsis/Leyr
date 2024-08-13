// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "AI/BaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API AAICharacter : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAICharacter();
	
	/** Combat Interface */
	virtual int32 GetCharacterLevel() override { return Level; }
	/** end Combat Interface */

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
};
