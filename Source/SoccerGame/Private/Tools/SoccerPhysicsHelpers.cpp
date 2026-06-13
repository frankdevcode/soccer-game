// Copyright Frank Dev Code. All Rights Reserved.

#include "Tools/SoccerPhysicsHelpers.h"
#include "Tools/SoccerMathHelpers.h"

FVector USoccerPhysicsHelpers::ApplyDrag(const FVector& Velocity, float DragCoefficient, float DeltaTime)
{
	const FVector Drag = Velocity * (DragCoefficient * DeltaTime);
	return Velocity - Drag;
}

FVector USoccerPhysicsHelpers::ApplyMagnus(const FVector& Velocity, const FVector& Spin, float MagnusCoefficient, float DeltaTime)
{
	if (Velocity.IsNearlyZero() || Spin.IsNearlyZero())
	{
		return FVector::ZeroVector;
	}

	const FVector MagnusForce = FVector::CrossProduct(Spin, Velocity) * MagnusCoefficient;
	return MagnusForce * DeltaTime;
}

FVector USoccerPhysicsHelpers::ApplyGroundFriction(const FVector& Velocity, float FrictionCoefficient, float DeltaTime)
{
	const FVector HorizontalVelocity = FVector(Velocity.X, Velocity.Y, 0.0f);
	const float Speed = HorizontalVelocity.Size();

	if (Speed <= KINDA_SMALL_NUMBER)
	{
		return Velocity;
	}

	const float FrictionAmount = FMath::Clamp(FrictionCoefficient * DeltaTime, 0.0f, 1.0f);
	const FVector Friction = HorizontalVelocity * FrictionAmount;
	FVector Result = Velocity - Friction;

	// preserve vertical component, if any
	Result.Z = Velocity.Z;
	return Result;
}

FVector USoccerPhysicsHelpers::ApplySpinDamping(const FVector& Spin, float DampingCoefficient, float DeltaTime)
{
	if (Spin.IsNearlyZero())
	{
		return FVector::ZeroVector;
	}

	const float Damping = FMath::Clamp(DampingCoefficient * DeltaTime, 0.0f, 1.0f);
	return FMath::Lerp(Spin, FVector::ZeroVector, Damping);
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
