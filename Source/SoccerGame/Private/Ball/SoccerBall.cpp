// Copyright Frank Dev Code. All Rights Reserved.

#include "Ball/SoccerBall.h"
#include "Ball/BallPhysicsComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/SoccerGameSettings.h"
#include "Tools/SoccerPhysicsHelpers.h"
#include "Audio/SoccerAudioComponent.h"

ASoccerBall::ASoccerBall()
	: KickForceMultiplier(1.0f)
	, PassForceMultiplier(0.75f)
	, HeaderForceMultiplier(0.85f)
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(11.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = CollisionSphere;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(RootComponent);
	BallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BallPhysicsComponent = CreateDefaultSubobject<UBallPhysicsComponent>(TEXT("BallPhysicsComponent"));
	AudioComponent = CreateDefaultSubobject<USoccerAudioComponent>(TEXT("SoccerAudioComponent"));
}

void ASoccerBall::BeginPlay()
{
	Super::BeginPlay();

	if (USoccerGameSettings* Settings = USoccerGameSettings::Get())
	{
		CollisionSphere->SetSphereRadius(Settings->BallRadius);
		if (BallPhysicsComponent)
		{
			BallPhysicsComponent->Mass = Settings->BallMass;
			BallPhysicsComponent->AirResistance = Settings->AirResistance;
			BallPhysicsComponent->MaxSpeed = Settings->MaxBallSpeed;
		}
	}
}

void ASoccerBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!BallPhysicsComponent)
	{
		return;
	}

	BallPhysicsComponent->SimulatePhysics(DeltaTime);
	const FVector DesiredLocation = GetActorLocation() + BallPhysicsComponent->GetVelocity() * DeltaTime;

	FHitResult Hit;
	SetActorLocation(DesiredLocation, true, &Hit);

	if (Hit.bBlockingHit)
	{
		HandleImpact(Hit);
	}
}

void ASoccerBall::ApplyKick(const FVector& Direction, float Power)
{
	if (!BallPhysicsComponent || Direction.IsNearlyZero())
	{
		return;
	}

	const FVector NormalizedDirection = Direction.GetSafeNormal();
	const float Strength = Power * KickForceMultiplier * USoccerGameSettings::Get()->KickPower;
	const FVector Impulse = NormalizedDirection * Strength;
	BallPhysicsComponent->ApplyImpulse(Impulse);

	const FVector Spin = FVector::CrossProduct(FVector::UpVector, NormalizedDirection) * 180.0f * Power;
	BallPhysicsComponent->SetSpin(Spin);
	if (AudioComponent)
	{
		AudioComponent->PlayKickSound();
	}
}

void ASoccerBall::ApplyPass(const FVector& Direction, float Power)
{
	if (!BallPhysicsComponent || Direction.IsNearlyZero())
	{
		return;
	}

	const FVector NormalizedDirection = Direction.GetSafeNormal();
	const float Strength = Power * PassForceMultiplier * USoccerGameSettings::Get()->KickPower;
	const FVector Impulse = NormalizedDirection * Strength;
	BallPhysicsComponent->ApplyImpulse(Impulse);

	const FVector Spin = FVector::CrossProduct(FVector::UpVector, NormalizedDirection) * 120.0f * Power;
	BallPhysicsComponent->SetSpin(Spin);
	if (AudioComponent)
	{
		AudioComponent->PlayPassSound();
	}
}

void ASoccerBall::ApplyHeader(const FVector& Direction, float Power)
{
	if (!BallPhysicsComponent || Direction.IsNearlyZero())
	{
		return;
	}

	const FVector HeaderDirection = (Direction.GetSafeNormal() + FVector::UpVector * 0.4f).GetSafeNormal();
	const float Strength = Power * HeaderForceMultiplier * USoccerGameSettings::Get()->KickPower;
	const FVector Impulse = HeaderDirection * Strength;
	BallPhysicsComponent->ApplyImpulse(Impulse);

	const FVector Spin = FVector::CrossProduct(HeaderDirection, FVector::UpVector) * 220.0f * Power;
	BallPhysicsComponent->SetSpin(Spin);
	if (AudioComponent)
	{
		AudioComponent->PlayHeaderSound();
	}
}

void ASoccerBall::ResetBall(const FVector& Location)
{
	SetActorLocation(Location);
	if (BallPhysicsComponent)
	{
		BallPhysicsComponent->SetVelocity(FVector::ZeroVector);
		BallPhysicsComponent->SetSpin(FVector::ZeroVector);
	}
}

void ASoccerBall::HandleImpact(const FHitResult& Hit)
{
	if (!BallPhysicsComponent)
	{
		return;
	}

	BallPhysicsComponent->ApplyCollisionResponseFromHit(Hit);
	if (AudioComponent)
	{
		AudioComponent->PlayCollisionSound();
	}
}
