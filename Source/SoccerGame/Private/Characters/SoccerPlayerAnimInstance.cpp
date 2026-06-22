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
	, bIsPerformingSpecialMove(false)
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

	// If a special move flag is set, override state
	if (bIsPerformingSpecialMove)
	{
		AnimationState = EPlayerAnimationState::SpecialMove;
		return;
	}

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

void USoccerPlayerAnimInstance::PlaySpecialMove(FName MoveName)
{
	bIsPerformingSpecialMove = true;
	MotionMatchPose = MoveName;
	AnimationState = EPlayerAnimationState::SpecialMove;
	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerAnimInstance] Playing special move: %s"), *MoveName.ToString());

	// Note: animation reset will be handled by gameplay code or animation notify in real setup.
}
