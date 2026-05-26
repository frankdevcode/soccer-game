// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoccerMatchManager.generated.h"

/**
 * @struct FSoccerMatchSummary
 * @brief Summary data for the current match session.
 */
USTRUCT(BlueprintType)
struct FSoccerMatchSummary
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Match")
	float ElapsedTime;

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Match")
	float Duration;

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Match")
	bool bIsActive;

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Match")
	bool bIsPaused;

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Match")
	int32 WinnerTeamId;

	FSoccerMatchSummary()
		: ElapsedTime(0.0f)
		, Duration(1200.0f)
		, bIsActive(false)
		, bIsPaused(false)
		, WinnerTeamId(-1)
	{
	}
};

/**
 * @class USoccerMatchManager
 * @brief Maintains current match timing and lifecycle state.
 */
UCLASS()
class SOCCERGAME_API USoccerMatchManager : public UObject
{
	GENERATED_BODY()

public:
	static USoccerMatchManager* Get();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	void Initialize(float MatchDuration);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	void Start();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	void Pause();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	void Resume();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	void End(int32 WinningTeamId = -1);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	void Tick(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	FSoccerMatchSummary GetSummary() const;

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	float GetRemainingTime() const;

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	void Reset();

private:
	static USoccerMatchManager* Instance;

	FSoccerMatchSummary CurrentSummary;
};
