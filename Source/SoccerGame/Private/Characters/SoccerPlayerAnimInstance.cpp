// Copyright Frank Dev Code. All Rights Reserved.

#include "Characters/SoccerPlayerAnimInstance.h"

USoccerPlayerAnimInstance::USoccerPlayerAnimInstance()
	: AnimationState(EPlayerAnimationState::Idle)
	, Speed(0.0f)
	, Direction(0.0f)
	, bIsMoving(false)
	, bIsSprinting(false)
	, bIsInAir(false)
	, bIsJumping(false)
	, bIsGoalkeeper(false)
	, bIsPerformingSave(false)
{
}

void USoccerPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AnimationState = EPlayerAnimationState::Idle;
	Speed = 0.0f;
	Direction = 0.0f;
	bIsMoving = false;
	bIsSprinting = false;
	bIsInAir = false;
	bIsJumping = false;
	bIsGoalkeeper = false;
	bIsPerformingSave = false;
}

void USoccerPlayerAnimInstance::UpdateState(float InSpeed, float InDirection, bool bMoving, bool bSprinting, bool bInAir, bool bIsGoalkeeperParam, bool bIsPerformingSaveParam, FName InMotionMatchPose)
{
	Speed = InSpeed;
	Direction = InDirection;
	bIsMoving = bMoving;
	bIsSprinting = bSprinting;
	bIsInAir = bInAir;
	bIsJumping = bInAir;
	bIsGoalkeeper = bIsGoalkeeperParam;
	bIsPerformingSave = bIsPerformingSaveParam;
	MotionMatchPose = InMotionMatchPose;

	if (bIsPerformingSave)
	{
		AnimationState = EPlayerAnimationState::Save;
	}
	else if (bIsInAir)
	{
		AnimationState = EPlayerAnimationState::Jump;
	}
	else if (bIsSprinting)
	{
		AnimationState = EPlayerAnimationState::Sprint;
	}
	else if (bIsMoving)
	{
		AnimationState = Speed > 300.0f ? EPlayerAnimationState::Run : EPlayerAnimationState::Walk;
	}
	else
	{
		AnimationState = EPlayerAnimationState::Idle;
	}
}
