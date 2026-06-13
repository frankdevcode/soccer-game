// Copyright Frank Dev Code. All Rights Reserved.

#include "Ball/BallPhysicsComponent.h"
#include "Tools/SoccerPhysicsHelpers.h"
#include "Core/SoccerGameSettings.h"

UBallPhysicsComponent::UBallPhysicsComponent()
	: Mass(0.43f)
	, AirResistance(0.1f)
	, MaxSpeed(5000.0f)
	, MagnusCoefficient(0.0005f)
	, GroundFriction(0.15f)
	, SpinDamping(0.08f)
	, Velocity(FVector::ZeroVector)
	, Spin(FVector::ZeroVector)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBallPhysicsComponent::BeginPlay()
{
	Super::BeginPlay();

	if (USoccerGameSettings* Settings = USoccerGameSettings::Get())
	{
		Mass = Settings->BallMass;
		AirResistance = Settings->AirResistance;
		MaxSpeed = Settings->MaxBallSpeed;
		MagnusCoefficient = Settings->BallMagnusCoefficient;
		GroundFriction = Settings->BallGroundFriction;
		SpinDamping = Settings->BallSpinDamping;
	}
}

void UBallPhysicsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SimulatePhysics(DeltaTime);
}

void UBallPhysicsComponent::SimulatePhysics(float DeltaTime)
{
	if (Velocity.IsNearlyZero() && Spin.IsNearlyZero())
	{
		Velocity = FVector::ZeroVector;
		Spin = FVector::ZeroVector;
		return;
	}

	Velocity = USoccerPhysicsHelpers::ApplyDrag(Velocity, AirResistance, DeltaTime);
	Velocity += USoccerPhysicsHelpers::ApplyMagnus(Velocity, Spin, MagnusCoefficient, DeltaTime);
	Velocity = USoccerPhysicsHelpers::ApplyGroundFriction(Velocity, GroundFriction, DeltaTime);
	Velocity = USoccerPhysicsHelpers::ClampVelocity(Velocity, MaxSpeed);

	Spin = USoccerPhysicsHelpers::ApplySpinDamping(Spin, SpinDamping, DeltaTime);

	if (Velocity.SizeSquared() < KINDA_SMALL_NUMBER)
	{
		Velocity = FVector::ZeroVector;
	}

	if (Spin.SizeSquared() < KINDA_SMALL_NUMBER)
	{
		Spin = FVector::ZeroVector;
	}
}

void UBallPhysicsComponent::ApplyImpulse(const FVector& Impulse)
{
	if (Mass <= KINDA_SMALL_NUMBER)
	{
		return;
	}

	Velocity += Impulse / Mass;
	Velocity = USoccerPhysicsHelpers::ClampVelocity(Velocity, MaxSpeed);
}

void UBallPhysicsComponent::SetSpin(const FVector& NewSpin)
{
	Spin = NewSpin;
}

void UBallPhysicsComponent::SetVelocity(const FVector& NewVelocity)
{
	Velocity = USoccerPhysicsHelpers::ClampVelocity(NewVelocity, MaxSpeed);
}
