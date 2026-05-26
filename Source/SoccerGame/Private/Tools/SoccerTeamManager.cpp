// Copyright Frank Dev Code. All Rights Reserved.

#include "Tools/SoccerTeamManager.h"

USoccerTeamManager* USoccerTeamManager::Instance = nullptr;

USoccerTeamManager* USoccerTeamManager::Get()
{
	if (!Instance)
	{
		Instance = NewObject<USoccerTeamManager>();
		Instance->AddToRoot();
	}

	return Instance;
}

bool USoccerTeamManager::RegisterTeam(const FSoccerTeamRecord& TeamRecord)
{
	if (TeamRecord.TeamId <= 0)
	{
		return false;
	}

	Teams.Add(TeamRecord.TeamId, TeamRecord);
	return true;
}

bool USoccerTeamManager::UpdateTeam(const FSoccerTeamRecord& TeamRecord)
{
	if (TeamRecord.TeamId <= 0)
	{
		return false;
	}

	Teams.Add(TeamRecord.TeamId, TeamRecord);
	return true;
}

void USoccerTeamManager::SetScore(int32 TeamId, int32 NewScore)
{
	if (FSoccerTeamRecord* Team = Teams.Find(TeamId))
	{
		Team->Score = NewScore;
	}
}

void USoccerTeamManager::IncrementFouls(int32 TeamId)
{
	if (FSoccerTeamRecord* Team = Teams.Find(TeamId))
	{
		Team->Fouls++;
	}
}

void USoccerTeamManager::SetPossession(int32 TeamId, int32 NewPossession)
{
	if (FSoccerTeamRecord* Team = Teams.Find(TeamId))
	{
		Team->Possession = NewPossession;
	}
}

FSoccerTeamRecord USoccerTeamManager::GetTeam(int32 TeamId) const
{
	if (const FSoccerTeamRecord* Team = Teams.Find(TeamId))
	{
		return *Team;
	}

	return FSoccerTeamRecord();
}

TArray<FSoccerTeamRecord> USoccerTeamManager::GetTeams() const
{
	TArray<FSoccerTeamRecord> TeamList;
	for (const TPair<int32, FSoccerTeamRecord>& Entry : Teams)
	{
		TeamList.Add(Entry.Value);
	}
	return TeamList;
}

void USoccerTeamManager::Clear()
{
	Teams.Empty();
}
