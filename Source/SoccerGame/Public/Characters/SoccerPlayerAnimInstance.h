// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SoccerPlayerAnimInstance.generated.h"

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
	void UpdateState(float InSpeed, float InDirection, bool bMoving, bool bSprinting, bool bInAir);

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
};
