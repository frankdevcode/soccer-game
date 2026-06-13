// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimSequenceBase.h"
#include "SoccerMotionMatchingComponent.generated.h"

USTRUCT(BlueprintType)
struct FMotionMatchingSample
{
	GENERATED_BODY()

	FMotionMatchingSample()
		: PoseName(NAME_None)
		, DesiredSpeed(0.0f)
		, DesiredDirection(0.0f)
		, bSprinting(false)
		, bInAir(false)
		, bGoalkeeper(false)
		, bSaveMotion(false)
		, AnimationSequence(nullptr)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|MotionMatching")
	FName PoseName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|MotionMatching")
	float DesiredSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|MotionMatching")
	float DesiredDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|MotionMatching")
	bool bSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|MotionMatching")
	bool bInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|MotionMatching")
	bool bGoalkeeper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|MotionMatching")
	bool bSaveMotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|MotionMatching")
	UAnimSequenceBase* AnimationSequence;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SOCCERGAME_API USoccerMotionMatchingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USoccerMotionMatchingComponent();

	UFUNCTION(BlueprintCallable, Category = "Soccer|MotionMatching")
	void UpdateMotionMatch(float Speed, float Direction, bool bSprinting, bool bInAir, bool bGoalkeeper, bool bSaveMotion);

	UFUNCTION(BlueprintCallable, Category = "Soccer|MotionMatching")
	FName GetBestMatchPose() const;

	UFUNCTION(BlueprintCallable, Category = "Soccer|MotionMatching")
	UAnimSequenceBase* GetBestMatchAnimation() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|MotionMatching")
	TArray<FMotionMatchingSample> MotionSamples;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|MotionMatching")
	float SpeedWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|MotionMatching")
	float DirectionWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|MotionMatching")
	float BooleanWeight;

private:
	FName BestMatchPose;
	UAnimSequenceBase* BestMatchAnimation;
};
