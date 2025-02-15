// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputConfig.h"
#include "BaseInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class LEYR_API UBaseInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType, typename ComboFuncType>
	void BindAbilityActions(const UInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc, ComboFuncType ComboFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType, typename ComboFuncType>
void UBaseInputComponent::BindAbilityActions(const UInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc, ComboFuncType ComboFunc)
{
	check(InputConfig);

	for (const FBaseInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
	for (const FBaseInputAction& Action : InputConfig->AbilityInputCombos)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (ComboFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ComboFunc, Action.InputTag);
			}
		}
	}
}
