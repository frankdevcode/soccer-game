// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoccerMathHelpers.generated.h"

/**
 * @class USoccerMathHelpers
 * @brief Utility methods for common math and formatting operations.
 */
UCLASS()
class SOCCERGAME_API USoccerMathHelpers : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	static float Clamp(float Value, float MinValue, float MaxValue);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	static FVector ClampMagnitude(const FVector& Value, float MaxMagnitude);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	static bool IsNearlyEqual(float A, float B, float Tolerance = KINDA_SMALL_NUMBER);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	static FVector CalculateKickVelocity(const FVector& Start, const FVector& Target, float Speed, float HeightOffset = 100.0f);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	static FString FormatTime(float Seconds);
};
