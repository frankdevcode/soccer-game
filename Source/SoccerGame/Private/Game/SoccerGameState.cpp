// Copyright Frank Dev Code. All Rights Reserved.

#include "Game/SoccerGameState.h"
#include "Characters/SoccerPlayerCharacter.h"
#include "Net/UnrealNetwork.h"

ASoccerGameState::ASoccerGameState()
	: BallPossessionTeamId(1)
	, MatchElapsedTime(0.0f)
	, bIsMatchActive(false)
{
	PrimaryActorTick.bCanEverTick = true;

	Team1Stats.TeamId = 1;
	Team2Stats.TeamId = 2;

	bReplicates = true;
}

void ASoccerGameState::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("[SoccerGameState] Game state initialized"));
}

void ASoccerGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoccerGameState::SetMatchActive(bool bActive)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		bIsMatchActive = bActive;
	}
}

void ASoccerGameState::SetMatchElapsedTime(float NewElapsedTime)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		MatchElapsedTime = NewElapsedTime;
	}
}

float ASoccerGameState::GetMatchElapsedTime() const
{
	return MatchElapsedTime;
}

bool ASoccerGameState::IsMatchActive() const
{
	return bIsMatchActive;
}

void ASoccerGameState::UpdateTeamScore(int32 TeamId, int32 NewScore)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (TeamId == 1)
		{
			Team1Stats.Score = NewScore;
		}
		else if (TeamId == 2)
		{
			Team2Stats.Score = NewScore;
		}

		UE_LOG(LogTemp, Warning, TEXT("[SoccerGameState] Team %d score updated to %d"),
			TeamId, NewScore);
	}
}

int32 ASoccerGameState::GetTeamScore(int32 TeamId) const
{
	return TeamId == 1 ? Team1Stats.Score : Team2Stats.Score;
}

void ASoccerGameState::IncrementTeamFouls(int32 TeamId)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (TeamId == 1)
		{
			Team1Stats.Fouls++;
		}
		else if (TeamId == 2)
		{
			Team2Stats.Fouls++;
		}

		UE_LOG(LogTemp, Warning, TEXT("[SoccerGameState] Team %d fouls: %d"),
			TeamId, TeamId == 1 ? Team1Stats.Fouls : Team2Stats.Fouls);
	}
}

int32 ASoccerGameState::GetTeamFouls(int32 TeamId) const
{
	return TeamId == 1 ? Team1Stats.Fouls : Team2Stats.Fouls;
}

void ASoccerGameState::SetBallPossession(int32 TeamId)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BallPossessionTeamId = TeamId;
	}
}

void ASoccerGameState::RegisterPlayer(ASoccerPlayerCharacter* Player, int32 TeamId)
{
	if (GetLocalRole() == ROLE_Authority && Player)
	{
		RegisteredPlayers.Add(Player);
		UE_LOG(LogTemp, Warning, TEXT("[SoccerGameState] Player registered - Team %d (Total: %d)"),
			TeamId, RegisteredPlayers.Num());
	}
}

void ASoccerGameState::UnregisterPlayer(ASoccerPlayerCharacter* Player)
{
	if (GetLocalRole() == ROLE_Authority && Player)
	{
		RegisteredPlayers.Remove(Player);
		UE_LOG(LogTemp, Warning, TEXT("[SoccerGameState] Player unregistered (Total: %d)"),
			RegisteredPlayers.Num());
	}
}

FTeamStats ASoccerGameState::GetTeamStats(int32 TeamId) const
{
	return TeamId == 1 ? Team1Stats : Team2Stats;
}

TArray<ASoccerPlayerCharacter*> ASoccerGameState::GetRegisteredPlayers() const
{
	return RegisteredPlayers;
}

void ASoccerGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASoccerGameState, Team1Stats);
	DOREPLIFETIME(ASoccerGameState, Team2Stats);
	DOREPLIFETIME(ASoccerGameState, BallPossessionTeamId);
	DOREPLIFETIME(ASoccerGameState, RegisteredPlayers);
	DOREPLIFETIME(ASoccerGameState, MatchElapsedTime);
	DOREPLIFETIME(ASoccerGameState, bIsMatchActive);
}
