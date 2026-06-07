// Copyright Frank Dev Code. All Rights Reserved.

#include "Ball/BallPhysicsComponent.h"
#include "Tools/SoccerPhysicsHelpers.h"
#include "Core/SoccerGameSettings.h"

UBallPhysicsComponent::UBallPhysicsComponent()
	: Mass(0.43f)
	, AirResistance(0.1f)
	, MaxSpeed(5000.0f)
	, Velocity(FVector::ZeroVector)
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
	}
}

void UBallPhysicsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SimulatePhysics(DeltaTime);
}

void UBallPhysicsComponent::SimulatePhysics(float DeltaTime)
{
	if (Velocity.IsNearlyZero())
	{
		Velocity = FVector::ZeroVector;
		return;
	}

	Velocity = USoccerPhysicsHelpers::ApplyDrag(Velocity, AirResistance, DeltaTime);
	Velocity = USoccerPhysicsHelpers::ClampVelocity(Velocity, MaxSpeed);

	if (Velocity.SizeSquared() < KINDA_SMALL_NUMBER)
	{
		Velocity = FVector::ZeroVector;
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

void UBallPhysicsComponent::SetVelocity(const FVector& NewVelocity)
{
	Velocity = USoccerPhysicsHelpers::ClampVelocity(NewVelocity, MaxSpeed);
}
