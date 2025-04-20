// @ Retropsis 2024-2025.

#include "Game/LeyrCheatManagerExtension.h"
#include "Game/BaseGameplayTags.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"

ULeyrCheatManagerExtension::ULeyrCheatManagerExtension()
{
#if UE_WITH_CHEAT_MANAGER
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		UCheatManager::RegisterForOnCheatManagerCreated(FOnCheatManagerCreated::FDelegate::CreateLambda(
		[](UCheatManager* CheatManager)
		{
			CheatManager->AddCheatManagerExtension(NewObject<ThisClass>(CheatManager));
		}));
	}
#endif
}

void ULeyrCheatManagerExtension::Invincibility(const bool bEnable) const
{
	if (const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		if(bEnable) PlayerCharacter->GetAbilitySystemComponent()->AddLooseGameplayTag(FBaseGameplayTags::Get().Invincibility);
		else PlayerCharacter->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FBaseGameplayTags::Get().Invincibility);
	}
}

void ULeyrCheatManagerExtension::Teleport(const FName Tag) const
{	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if(Actors.Num() > 0)
	{
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if(PlayerStart->PlayerStartTag == Tag)
				{
					if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
					{
						PlayerCharacter->SetActorLocation(FVector{ PlayerStart->GetActorLocation().X, 0.f, PlayerStart->GetActorLocation().Z + 50.f });
					}
					break;
				}
			}
		}
	}
}
