// Copyright Frank Dev Code. All Rights Reserved.

#include "Tools/SoccerPlayerManager.h"

USoccerPlayerManager* USoccerPlayerManager::Instance = nullptr;

USoccerPlayerManager* USoccerPlayerManager::Get()
{
	if (!Instance)
	{
		Instance = NewObject<USoccerPlayerManager>();
		Instance->AddToRoot();
	}

	return Instance;
}

bool USoccerPlayerManager::RegisterPlayer(const FSoccerPlayerRecord& PlayerRecord)
{
	if (PlayerRecord.PlayerId.IsEmpty())
	{
		return false;
	}

	Players.Add(PlayerRecord.PlayerId, PlayerRecord);
	return true;
}

bool USoccerPlayerManager::UpdatePlayer(const FSoccerPlayerRecord& PlayerRecord)
{
	if (PlayerRecord.PlayerId.IsEmpty())
	{
		return false;
	}

	Players.Add(PlayerRecord.PlayerId, PlayerRecord);
	return true;
}

bool USoccerPlayerManager::UnregisterPlayer(const FString& PlayerId)
{
	return Players.Remove(PlayerId) > 0;
}

FSoccerPlayerRecord USoccerPlayerManager::GetPlayer(const FString& PlayerId) const
{
	if (const FSoccerPlayerRecord* Found = Players.Find(PlayerId))
	{
		return *Found;
	}

	return FSoccerPlayerRecord();
}

TArray<FSoccerPlayerRecord> USoccerPlayerManager::GetPlayersForTeam(int32 TeamId) const
{
	TArray<FSoccerPlayerRecord> TeamPlayers;

	for (const TPair<FString, FSoccerPlayerRecord>& Entry : Players)
	{
		if (Entry.Value.TeamId == TeamId)
		{
			TeamPlayers.Add(Entry.Value);
		}
	}

	return TeamPlayers;
}

int32 USoccerPlayerManager::GetPlayerCount() const
{
	return Players.Num();
}

void USoccerPlayerManager::Clear()
{
	Players.Empty();
}
