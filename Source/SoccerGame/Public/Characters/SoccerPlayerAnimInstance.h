// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SoccerPlayerAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EPlayerAnimationState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Walk UMETA(DisplayName = "Walk"),
	Run UMETA(DisplayName = "Run"),
	Sprint UMETA(DisplayName = "Sprint"),
	Jump UMETA(DisplayName = "Jump"),
	Save UMETA(DisplayName = "Save"),
	Fall UMETA(DisplayName = "Fall")
};

/**
 * @class USoccerPlayerAnimInstance
 * @brief Animation instance for soccer player state-driven animations.
 */
UCLASS(Blueprintable, BlueprintType)
class SOCCERGAME_API USoccerPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	USoccerPlayerAnimInstance();

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "Soccer|Animation")
	void UpdateState(float InSpeed, float InDirection, bool bMoving, bool bSprinting, bool bInAir, bool bIsGoalkeeper, bool bIsPerformingSave);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|Animation")
	EPlayerAnimationState AnimationState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|Animation")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|Animation")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|Animation")
	bool bIsMoving;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|Animation")
	bool bIsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|Animation")
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|Animation")
	bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|Animation")
	bool bIsGoalkeeper;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|Animation")
	bool bIsPerformingSave;
};
