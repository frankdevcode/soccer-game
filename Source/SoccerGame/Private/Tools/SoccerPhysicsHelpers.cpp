// Copyright Frank Dev Code. All Rights Reserved.

#include "Tools/SoccerPhysicsHelpers.h"

FVector USoccerPhysicsHelpers::ApplyDrag(const FVector& Velocity, float DragCoefficient, float DeltaTime)
{
	const FVector Drag = Velocity * (DragCoefficient * DeltaTime);
	return Velocity - Drag;
}

FVector USoccerPhysicsHelpers::ClampVelocity(const FVector& Velocity, float MaxSpeed)
{
	return USoccerMathHelpers::ClampMagnitude(Velocity, MaxSpeed);
}

float USoccerPhysicsHelpers::CalculateImpactForce(float KickPower, float BallMass)
{
	if (BallMass <= 0.0f)
	{
		return 0.0f;
	}

	return KickPower / BallMass;
}
