// @ Retropsis 2024-2025.

#include "Component/MultiPartComponent.h"

void UMultiPartComponent::AttackComponentsToSkeleton(TArray<USceneComponent*> Components)
{
	if (Components.Num() == 0 || GetSkinnedAsset()->GetActiveSocketList().Num() == 0) return;

	const int32 Max = Components.Num() < GetAllSocketNames().Num() ? Components.Num() : GetAllSocketNames().Num();
	for (int i = 0; i < Max; ++i)
	{
		if (Components[i])
		{
			Components[i]->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform, GetAllSocketNames()[i]);
		}
	}
}
