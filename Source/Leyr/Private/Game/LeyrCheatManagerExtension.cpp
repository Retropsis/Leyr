// @ Retropsis 2024-2025.

#include "Game/LeyrCheatManagerExtension.h"
#include "Game/BaseGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"

void ULeyrCheatManagerExtension::Invincibility(const bool bEnable) const
{
	if (const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		if(bEnable) PlayerCharacter->GetAbilitySystemComponent()->AddLooseGameplayTag(FBaseGameplayTags::Get().Invincibility);
		else PlayerCharacter->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FBaseGameplayTags::Get().Invincibility);
	}
}
