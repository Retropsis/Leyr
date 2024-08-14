// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LeyrAbilitySystemLibrary.generated.h"

class UAttributeMenuWidgetController;
class APlayerHUD;
struct FWidgetControllerParams;
class UOverlayWidgetController;

/**
 * 
 */
UCLASS()
class LEYR_API ULeyrAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category="LeyrAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="LeyrAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static  UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
	// UFUNCTION(BlueprintPure, Category="LeyrAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	// static  USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="LeyrAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static  bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, APlayerHUD*& OutPlayerHUD);
};
