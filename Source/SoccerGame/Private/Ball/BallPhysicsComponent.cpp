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
	, BounceCoefficient(0.72f)
	, CollisionFriction(0.22f)
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
		BounceCoefficient = Settings->BallBounceCoefficient;
		CollisionFriction = Settings->BallCollisionFriction;
		FloorBounceCoefficient = Settings->FloorBounceCoefficient;
		FloorFriction = Settings->FloorFriction;
		WallBounceCoefficient = Settings->WallBounceCoefficient;
		WallFriction = Settings->WallFriction;
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

void UBallPhysicsComponent::ApplyCollisionResponseFromHit(const FHitResult& Hit)
{
	float EffectiveBounce = BounceCoefficient;
	float EffectiveFriction = CollisionFriction;

	if (Hit.Component.IsValid())
	{
		const FString ComponentName = Hit.Component->GetName();

		if (ComponentName.Contains(TEXT("FieldMesh")))
		{
			EffectiveBounce = FloorBounceCoefficient;
			EffectiveFriction = FloorFriction;
		}
		else if (ComponentName.Contains(TEXT("Boundary")) || ComponentName.Contains(TEXT("Goal")))
		{
			EffectiveBounce = WallBounceCoefficient;
			EffectiveFriction = WallFriction;
		}
	}

	ApplyCollisionResponse(Hit.Normal, EffectiveBounce, EffectiveFriction);
}

void UBallPhysicsComponent::ApplyCollisionResponse(const FVector& Normal, float BounceCoefficientOverride, float CollisionFrictionOverride)
{
	if (Velocity.IsNearlyZero())
	{
		return;
	}

	const float EffectiveBounce = BounceCoefficientOverride >= 0.0f ? BounceCoefficientOverride : BounceCoefficient;
	const float EffectiveFriction = CollisionFrictionOverride >= 0.0f ? CollisionFrictionOverride : CollisionFriction;
	const FVector SafeNormal = Normal.GetSafeNormal();
	Velocity = USoccerPhysicsHelpers::ReflectVelocity(Velocity, SafeNormal, EffectiveBounce, EffectiveFriction);
	Velocity = USoccerPhysicsHelpers::ClampVelocity(Velocity, MaxSpeed);
	Spin *= FMath::Clamp(1.0f - EffectiveFriction * 0.5f, 0.0f, 1.0f);
}

void UBallPhysicsComponent::SetSpin(const FVector& NewSpin)
{
	Spin = NewSpin;
}

void UBallPhysicsComponent::SetVelocity(const FVector& NewVelocity)
{
	Velocity = USoccerPhysicsHelpers::ClampVelocity(NewVelocity, MaxSpeed);
}
