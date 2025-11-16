// @ Retropsis 2024-2025.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MultiPartAIInterface.generated.h"

class UPaperFlipbook;
class UPaperFlipbookComponent;

UENUM(BlueprintType)
enum class EMultiPartAnimationState : uint8
{
	None,
	Idle,
	Leap,
	Swim,
	Attack,
	Destroyed,
};

USTRUCT(BlueprintType)
struct FFlipbookPair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EMultiPartAnimationState State = EMultiPartAnimationState::None;

	UPROPERTY(EditAnywhere)
	bool bForeground = true;

	UPROPERTY(EditAnywhere)
	UPaperFlipbookComponent* FlipbookComponent = nullptr;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> ForwardFlipbook = nullptr;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> FlippedFlipbook = nullptr;
};

enum class EDirection : uint8;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMultiPartAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEYR_API IMultiPartAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ChangeDirection(EDirection NewDirection);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimInstance* GetMultiPartAnimInstance();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ChangeMultiPartAnimInstance(TSubclassOf<UAnimInstance> NewInstance);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateMultiPartAnimInstances(EMultiPartAnimationState State);
};
