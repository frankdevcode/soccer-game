// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SoccerGameState.generated.h"

class ASoccerPlayerCharacter;
class UBallPhysicsComponent;

/**
 * @struct FTeamStats
 * @brief Team statistics container
 */
USTRUCT(BlueprintType)
struct FTeamStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Stats")
	int32 TeamId;

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Stats")
	int32 Score;

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Stats")
	int32 Fouls;

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Stats")
	int32 Possession;

	FTeamStats()
		: TeamId(0), Score(0), Fouls(0), Possession(0)
	{
	}
};

/**
 * @class ASoccerGameState
 * @brief Manages global game state and statistics
 * 
 * Replicates over network, maintains team stats, player data,
 * and match statistics. Accessible by all clients.
 */
UCLASS()
class SOCCERGAME_API ASoccerGameState : public AGameState
{
	GENERATED_BODY()

public:
	ASoccerGameState();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Team statistics
	UFUNCTION(BlueprintCallable, Category = "Soccer|Stats")
	void UpdateTeamScore(int32 TeamId, int32 NewScore);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Stats")
	int32 GetTeamScore(int32 TeamId) const;

	UFUNCTION(BlueprintCallable, Category = "Soccer|Stats")
	void IncrementTeamFouls(int32 TeamId);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Stats")
	int32 GetTeamFouls(int32 TeamId) const;

	// Possession tracking
	UFUNCTION(BlueprintCallable, Category = "Soccer|Stats")
	void SetBallPossession(int32 TeamId);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Stats")
	int32 GetBallPossession() const { return BallPossessionTeamId; }

	// Player management
	UFUNCTION(BlueprintCallable, Category = "Soccer|Players")
	void RegisterPlayer(class ASoccerPlayerCharacter* Player, int32 TeamId);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Players")
	void UnregisterPlayer(class ASoccerPlayerCharacter* Player);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Players")
	int32 GetPlayerCount() const { return RegisteredPlayers.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Soccer|Players")
	TArray<class ASoccerPlayerCharacter*> GetRegisteredPlayers() const;

	// Match state sync
	UFUNCTION(BlueprintCallable, Category = "Soccer|Match")
	void SetMatchActive(bool bActive);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Match")
	void SetMatchElapsedTime(float NewElapsedTime);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Match")
	float GetMatchElapsedTime() const;

	UFUNCTION(BlueprintCallable, Category = "Soccer|Match")
	bool IsMatchActive() const;

	// Getters
	UFUNCTION(BlueprintCallable, Category = "Soccer|Stats")
	FTeamStats GetTeamStats(int32 TeamId) const;

protected:
	// Team statistics
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Soccer|Stats")
	FTeamStats Team1Stats;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Soccer|Stats")
	FTeamStats Team2Stats;

	// Ball possession
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Soccer|Stats")
	int32 BallPossessionTeamId;

	// Players
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Soccer|Players")
	TArray<class ASoccerPlayerCharacter*> RegisteredPlayers;

	// Match timing
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Soccer|Match")
	float MatchElapsedTime;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Soccer|Match")
	bool bIsMatchActive;

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
