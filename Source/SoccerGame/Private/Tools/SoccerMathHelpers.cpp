// Copyright Frank Dev Code. All Rights Reserved.

#include "Tools/SoccerMathHelpers.h"

float USoccerMathHelpers::Clamp(float Value, float MinValue, float MaxValue)
{
	return FMath::Clamp(Value, MinValue, MaxValue);
}

FVector USoccerMathHelpers::ClampMagnitude(const FVector& Value, float MaxMagnitude)
{
	if (Value.Size() <= MaxMagnitude)
	{
		return Value;
	}

	return Value.GetSafeNormal() * MaxMagnitude;
}

bool USoccerMathHelpers::IsNearlyEqual(float A, float B, float Tolerance)
{
	return FMath::IsNearlyEqual(A, B, Tolerance);
}

FVector USoccerMathHelpers::CalculateKickVelocity(const FVector& Start, const FVector& Target, float Speed, float HeightOffset)
{
	const FVector Direction = (Target - Start);
	const FVector AdjustedTarget = Target + FVector(0.0f, 0.0f, HeightOffset);
	const FVector KickDirection = (AdjustedTarget - Start).GetSafeNormal();

	return KickDirection * Speed;
}

FString USoccerMathHelpers::FormatTime(float Seconds)
{
	const int32 TotalSeconds = FMath::Max(0, FMath::RoundToInt(Seconds));
	const int32 Minutes = TotalSeconds / 60;
	const int32 RemainingSeconds = TotalSeconds % 60;

	return FString::Printf(TEXT("%02d:%02d"), Minutes, RemainingSeconds);
}
