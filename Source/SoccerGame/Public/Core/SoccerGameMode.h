// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Tools/SoccerTrainingManager.h"
#include "Tools/SoccerTournamentManager.h"
#include "SoccerGameMode.generated.h"

class ASoccerPlayerCharacter;
class USoccerGameState;

/**
 * @class ASoccerGameMode
 * @brief Core game mode for Soccer-Game
 * 
 * Manages overall game flow, match rules, and game state.
 * Handles player spawning, match initialization, and match completion.
 */
UCLASS()
class SOCCERGAME_API ASoccerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASoccerGameMode();

	// Game initialization
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Match management
	/**
	 * @brief Initialize a new match with specified parameters
	 * @param PlayerCount Number of players per team
	 * @param MatchDurationSeconds Total match duration
	 */
	UFUNCTION(BlueprintCallable, Category = "Soccer|Match")
	void InitializeMatch(int32 PlayerCount = 5, float MatchDurationSeconds = 1200.0f);

	/**
	 * @brief Start the match
	 */
	UFUNCTION(BlueprintCallable, Category = "Soccer|Match")
	void StartMatch();

	/**
	 * @brief Start a quick match with automatic player spawning for testing
	 * @param TeamSize Number of players per team (default 5)
	 * @param Duration Match duration in seconds
	 * @param bAutoStart If true, the match will start immediately after setup
	 */
	UFUNCTION(BlueprintCallable, Category = "Soccer|Match")
	void StartQuickMatch(int32 TeamSize = 5, float Duration = 600.0f, bool bAutoStart = true);

	/**
	 * @brief Start Career/Professional mode for a player profile
	 * @param PlayerName Name for the career player
	 * @param bAutoStartMatch If true, begin a quick match after profile creation
	 */
	UFUNCTION(BlueprintCallable, Category = "Soccer|Career")
	void StartCareerMode(const FString& PlayerName = TEXT("Player"), bool bAutoStartMatch = true);

	/**
	 * @brief Start the training mode with a single drill
	 */
	UFUNCTION(BlueprintCallable, Category = "Soccer|Training")
	void StartTrainingMode(ETrainingDrillType DrillType = ETrainingDrillType::Shooting);

	/**
	 * @brief End the training session and return to menu or quick match state
	 */
	UFUNCTION(BlueprintCallable, Category = "Soccer|Training")
	void EndTrainingMode();

	/**
	 * @brief Start a tournament mode with selected teams
	 */
	UFUNCTION(BlueprintCallable, Category = "Soccer|Tournament")
	void StartTournamentMode(EGameTournamentType TournamentType = EGameTournamentType::Knockout, const TArray<int32>& TeamIds = TArray<int32>());

	/**
	 * @brief End the tournament session
	 */
	UFUNCTION(BlueprintCallable, Category = "Soccer|Tournament")
	void EndTournamentMode();

	/**
	 * @brief End the match
	 * @param WinningTeamId ID of winning team (-1 for draw)
	 */
	UFUNCTION(BlueprintCallable, Category = "Soccer|Match")
	void EndMatch(int32 WinningTeamId = -1);

	/**
	 * @brief Pause the match
	 */
	UFUNCTION(BlueprintCallable, Category = "Soccer|Match")
	void PauseMatch();

	/**
	 * @brief Resume the match
	 */
	UFUNCTION(BlueprintCallable, Category = "Soccer|Match")
	void ResumeMatch();

	// Getters
	UFUNCTION(BlueprintCallable, Category = "Soccer|Match")
	bool IsMatchActive() const { return bMatchActive; }

	UFUNCTION(BlueprintCallable, Category = "Soccer|Match")
	bool IsMatchPaused() const { return bMatchPaused; }

	UFUNCTION(BlueprintCallable, Category = "Soccer|Match")
	float GetMatchElapsedTime() const { return MatchElapsedTime; }

	UFUNCTION(BlueprintCallable, Category = "Soccer|Match")
	float GetMatchDuration() const { return MatchDurationSeconds; }

	UFUNCTION(BlueprintCallable, Category = "Soccer|Match")
	int32 GetPlayersPerTeam() const { return PlayersPerTeam; }

protected:
	// Match state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Match")
	bool bMatchActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Match")
	bool bMatchPaused;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Match")
	float MatchElapsedTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Match")
	float MatchDurationSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Match")
	int32 PlayersPerTeam;

	// Player spawning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Spawning")
	TArray<FVector> Team1SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Spawning")
	TArray<FVector> Team2SpawnPoints;

private:
	// Internal match management
	void UpdateMatchTime(float DeltaTime);
	void CheckMatchConditions();
};
