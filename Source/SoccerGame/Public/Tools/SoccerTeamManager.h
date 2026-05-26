// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoccerTeamManager.generated.h"

/**
 * @struct FSoccerTeamRecord
 * @brief Runtime team metadata used by match and UI systems.
 */
USTRUCT(BlueprintType)
struct FSoccerTeamRecord
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Teams")
	int32 TeamId;

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Teams")
	FString TeamName;

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Teams")
	int32 Score;

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Teams")
	int32 Fouls;

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Teams")
	int32 Possession;

	FSoccerTeamRecord()
		: TeamId(0)
		, Score(0)
		, Fouls(0)
		, Possession(0)
	{
	}
};

/**
 * @class USoccerTeamManager
 * @brief Tracks team state and summary information.
 */
UCLASS()
class SOCCERGAME_API USoccerTeamManager : public UObject
{
	GENERATED_BODY()

public:
	static USoccerTeamManager* Get();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	bool RegisterTeam(const FSoccerTeamRecord& TeamRecord);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	bool UpdateTeam(const FSoccerTeamRecord& TeamRecord);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	void SetScore(int32 TeamId, int32 NewScore);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	void IncrementFouls(int32 TeamId);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	void SetPossession(int32 TeamId, int32 NewPossession);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	FSoccerTeamRecord GetTeam(int32 TeamId) const;

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	TArray<FSoccerTeamRecord> GetTeams() const;

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	void Clear();

private:
	static USoccerTeamManager* Instance;

	TMap<int32, FSoccerTeamRecord> Teams;
};
