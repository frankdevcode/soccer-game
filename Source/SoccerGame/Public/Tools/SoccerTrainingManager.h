// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoccerTrainingManager.generated.h"

UENUM(BlueprintType)
enum class ETrainingDrillType : uint8
{
	Shooting UMETA(DisplayName = "Shooting"),
	Passing UMETA(DisplayName = "Passing"),
	Dribbling UMETA(DisplayName = "Dribbling"),
	Goalkeeping UMETA(DisplayName = "Goalkeeping")
};

USTRUCT(BlueprintType)
struct FTrainingDrill
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Training")
	FName DrillName;

	UPROPERTY(BlueprintReadWrite, Category = "Training")
	ETrainingDrillType DrillType;

	UPROPERTY(BlueprintReadWrite, Category = "Training")
	int32 TargetScore;

	UPROPERTY(BlueprintReadWrite, Category = "Training")
	bool bIsCompleted;

	FTrainingDrill()
		: DrillName(NAME_None)
		, DrillType(ETrainingDrillType::Shooting)
		, TargetScore(0)
		, bIsCompleted(false)
	{
	}
};

UCLASS()
class SOCCERGAME_API USoccerTrainingManager : public UObject
{
	GENERATED_BODY()

public:
	static USoccerTrainingManager* Get();

	UFUNCTION(BlueprintCallable, Category = "Training")
	void StartTrainingSession(ETrainingDrillType DrillType);

	UFUNCTION(BlueprintCallable, Category = "Training")
	FTrainingDrill GetCurrentDrill() const;

	UFUNCTION(BlueprintCallable, Category = "Training")
	void CompleteDrill();

	UFUNCTION(BlueprintCallable, Category = "Training")
	void ResetTrainingSession();

private:
	static USoccerTrainingManager* Instance;

	FTrainingDrill CurrentDrill;
};
