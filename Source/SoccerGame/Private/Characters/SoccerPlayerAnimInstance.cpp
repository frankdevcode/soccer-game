// Copyright Frank Dev Code. All Rights Reserved.

#include "Characters/SoccerPlayerAnimInstance.h"

USoccerPlayerAnimInstance::USoccerPlayerAnimInstance()
	: Speed(0.0f)
	, Direction(0.0f)
	, bIsMoving(false)
	, bIsSprinting(false)
	, bIsInAir(false)
	, bIsJumping(false)
{
}

void USoccerPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Speed = 0.0f;
	Direction = 0.0f;
	bIsMoving = false;
	bIsSprinting = false;
	bIsInAir = false;
	bIsJumping = false;
}

void USoccerPlayerAnimInstance::UpdateState(float InSpeed, float InDirection, bool bMoving, bool bSprinting, bool bInAir)
{
	Speed = InSpeed;
	Direction = InDirection;
	bIsMoving = bMoving;
	bIsSprinting = bSprinting;
	bIsInAir = bInAir;
	bIsJumping = bInAir;
}
