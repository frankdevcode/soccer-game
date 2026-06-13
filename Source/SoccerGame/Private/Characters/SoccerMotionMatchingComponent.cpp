// Copyright Frank Dev Code. All Rights Reserved.

#include "Characters/SoccerMotionMatchingComponent.h"
#include "Math/UnrealMathUtility.h"

USoccerMotionMatchingComponent::USoccerMotionMatchingComponent()
	: SpeedWeight(1.0f)
	, DirectionWeight(1.0f)
	, BooleanWeight(0.5f)
	, BestMatchPose(NAME_None)
	, BestMatchAnimation(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USoccerMotionMatchingComponent::UpdateMotionMatch(float Speed, float Direction, bool bSprinting, bool bInAir, bool bGoalkeeper, bool bSaveMotion)
{
	float BestScore = TNumericLimits<float>::Max();
	BestMatchPose = NAME_None;
	BestMatchAnimation = nullptr;

	for (const FMotionMatchingSample& Sample : MotionSamples)
	{
		float Score = 0.0f;
		Score += FMath::Abs(Speed - Sample.DesiredSpeed) * SpeedWeight;

		const float AngleDelta = FMath::Abs(FMath::UnwindDegrees(Direction - Sample.DesiredDirection)) / 180.0f;
		Score += AngleDelta * DirectionWeight;

		Score += (bSprinting == Sample.bSprinting ? 0.0f : 1.0f) * BooleanWeight;
		Score += (bInAir == Sample.bInAir ? 0.0f : 1.0f) * BooleanWeight;
		Score += (bGoalkeeper == Sample.bGoalkeeper ? 0.0f : 1.0f) * BooleanWeight;
		Score += (bSaveMotion == Sample.bSaveMotion ? 0.0f : 1.0f) * BooleanWeight;

		if (Score < BestScore)
		{
			BestScore = Score;
			BestMatchPose = Sample.PoseName;
			BestMatchAnimation = Sample.AnimationSequence;
		}
	}
}

FName USoccerMotionMatchingComponent::GetBestMatchPose() const
{
	return BestMatchPose;
}

UAnimSequenceBase* USoccerMotionMatchingComponent::GetBestMatchAnimation() const
{
	return BestMatchAnimation;
}
