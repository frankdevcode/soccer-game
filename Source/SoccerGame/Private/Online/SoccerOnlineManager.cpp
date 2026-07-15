// Copyright Frank Dev Code. All Rights Reserved.

#include "Online/SoccerOnlineManager.h"

USoccerOnlineManager::USoccerOnlineManager()
	: MatchmakingState(ESoccerMatchmakingState::Idle)
	, ActiveMatchId(TEXT("local-match"))
	, bConnected(false)
{
	LocalProfile.PlayerId = TEXT("local-player");
	LocalProfile.DisplayName = TEXT("Player");
	LocalProfile.Level = 1;
	LocalProfile.XP = 0;
	LocalProfile.Wins = 0;
	LocalProfile.Losses = 0;
}

void USoccerOnlineManager::Initialize()
{
	MatchmakingState = ESoccerMatchmakingState::Idle;
	bConnected = false;
	UE_LOG(LogTemp, Warning, TEXT("[SoccerOnlineManager] Online subsystem initialized"));
}

void USoccerOnlineManager::StartMatchmaking(const FString& PlayerName)
{
	LocalProfile.DisplayName = PlayerName.IsEmpty() ? LocalProfile.DisplayName : PlayerName;
	MatchmakingState = ESoccerMatchmakingState::Searching;
	UE_LOG(LogTemp, Warning, TEXT("[SoccerOnlineManager] Matchmaking started for %s"), *LocalProfile.DisplayName);
}

void USoccerOnlineManager::StopMatchmaking()
{
	MatchmakingState = ESoccerMatchmakingState::Idle;
	UE_LOG(LogTemp, Warning, TEXT("[SoccerOnlineManager] Matchmaking stopped"));
}

void USoccerOnlineManager::ConnectToMatch()
{
	MatchmakingState = ESoccerMatchmakingState::Matched;
	bConnected = true;
	ActiveMatchId = FString::Printf(TEXT("match-%d"), FMath::RandRange(1000, 9999));
	MatchmakingState = ESoccerMatchmakingState::Connected;
	UE_LOG(LogTemp, Warning, TEXT("[SoccerOnlineManager] Connected to match %s"), *ActiveMatchId);
}

void USoccerOnlineManager::SyncGameState(const FString& MatchId, float MatchTime, int32 TeamAScore, int32 TeamBScore)
{
	UE_LOG(LogTemp, Warning, TEXT("[SoccerOnlineManager] Sync match %s time %.1f score %d-%d"), *MatchId, MatchTime, TeamAScore, TeamBScore);
}

void USoccerOnlineManager::AwardXP(int32 Amount)
{
	LocalProfile.XP += Amount;
	if (LocalProfile.XP >= 100)
	{
		LocalProfile.Level += 1;
		LocalProfile.XP -= 100;
	}
	UE_LOG(LogTemp, Warning, TEXT("[SoccerOnlineManager] XP awarded: %d, level %d"), Amount, LocalProfile.Level);
}
