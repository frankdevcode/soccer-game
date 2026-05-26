// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoccerPhysicsHelpers.generated.h"

/**
 * @class USoccerPhysicsHelpers
 * @brief Low-level physics helpers for ball and movement calculations.
 */
UCLASS()
class SOCCERGAME_API USoccerPhysicsHelpers : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	static FVector ApplyDrag(const FVector& Velocity, float DragCoefficient, float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	static FVector ClampVelocity(const FVector& Velocity, float MaxSpeed);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	static float CalculateImpactForce(float KickPower, float BallMass);
};
