// Copyright Frank Dev Code. All Rights Reserved.

#include "Tools/SoccerTournamentManager.h"
#include "Tools/SoccerTeamManager.h"

USoccerTournamentManager* USoccerTournamentManager::Instance = nullptr;

USoccerTournamentManager* USoccerTournamentManager::Get()
{
    if (!Instance)
    {
        Instance = NewObject<USoccerTournamentManager>();
        Instance->AddToRoot();
    }
    return Instance;
}

bool USoccerTournamentManager::InitializeTournament(EGameTournamentType InTournamentType, const TArray<int32>& TeamIds)
{
    if (TeamIds.Num() < 2)
    {
        return false;
    }

    TournamentType = InTournamentType;
    RegisteredTeams = TeamIds;
    CurrentRound = 1;
    bIsActive = false;
    TournamentWinnerId = -1;
    Matches.Empty();

    CreateInitialBracket();
    return true;
}

bool USoccerTournamentManager::StartTournament()
{
    if (RegisteredTeams.Num() < 2 || Matches.Num() == 0)
    {
        return false;
    }

    bIsActive = true;
    CurrentRound = 1;
    return true;
}

bool USoccerTournamentManager::RecordMatchResult(int32 MatchId, int32 TeamAScore, int32 TeamBScore)
{
    for (FTournamentMatch& Match : Matches)
    {
        if (Match.MatchId != MatchId || Match.bCompleted)
        {
            continue;
        }

        Match.TeamAScore = TeamAScore;
        Match.TeamBScore = TeamBScore;
        Match.bCompleted = true;
        Match.WinnerTeamId = (TeamAScore >= TeamBScore) ? Match.TeamAId : Match.TeamBId;

        UE_LOG(LogTemp, Warning, TEXT("[Tournament] Match %d completed: %d vs %d - winner %d"), MatchId, TeamAScore, TeamBScore, Match.WinnerTeamId);

        if (TournamentType == EGameTournamentType::Knockout)
        {
            if (AreAllMatchesInRoundComplete(CurrentRound))
            {
                AdvanceKnockoutRound();
            }
        }
        else if (TournamentType == EGameTournamentType::League)
        {
            if (AreAllMatchesInRoundComplete(CurrentRound))
            {
                bIsActive = false;
                TournamentWinnerId = Match.WinnerTeamId; // placeholder: top scorer logic later
            }
        }

        return true;
    }

    return false;
}

TArray<FTournamentMatch> USoccerTournamentManager::GetMatches() const
{
    return Matches;
}

FTournamentSummary USoccerTournamentManager::GetSummary() const
{
    FTournamentSummary Summary;
    Summary.TournamentType = TournamentType;
    Summary.TotalTeams = RegisteredTeams.Num();
    Summary.CurrentRound = CurrentRound;
    Summary.bIsActive = bIsActive;
    Summary.TournamentWinnerId = TournamentWinnerId;
    return Summary;
}

bool USoccerTournamentManager::IsTournamentComplete() const
{
    return !bIsActive && TournamentWinnerId != -1;
}

void USoccerTournamentManager::ResetTournament()
{
    RegisteredTeams.Empty();
    Matches.Empty();
    CurrentRound = 0;
    bIsActive = false;
    TournamentWinnerId = -1;
}

void USoccerTournamentManager::CreateInitialBracket()
{
    if (TournamentType == EGameTournamentType::Knockout)
    {
        CreateKnockoutBracket(RegisteredTeams);
    }
    else
    {
        CreateLeagueBracket(RegisteredTeams);
    }
}

void USoccerTournamentManager::CreateKnockoutBracket(const TArray<int32>& TeamIds)
{
    Matches.Empty();
    int32 MatchId = 1;
    for (int32 i = 0; i + 1 < TeamIds.Num(); i += 2)
    {
        FTournamentMatch Match;
        Match.MatchId = MatchId++;
        Match.TeamAId = TeamIds[i];
        Match.TeamBId = TeamIds[i + 1];
        Match.Round = CurrentRound;
        Matches.Add(Match);
    }

    if (TeamIds.Num() % 2 == 1)
    {
        FTournamentMatch ByeMatch;
        ByeMatch.MatchId = MatchId++;
        ByeMatch.TeamAId = TeamIds.Last();
        ByeMatch.TeamBId = 0;
        ByeMatch.WinnerTeamId = TeamIds.Last();
        ByeMatch.bCompleted = true;
        ByeMatch.Round = CurrentRound;
        Matches.Add(ByeMatch);
    }
}

void USoccerTournamentManager::CreateLeagueBracket(const TArray<int32>& TeamIds)
{
    Matches.Empty();
    int32 MatchId = 1;
    for (int32 i = 0; i < TeamIds.Num(); ++i)
    {
        for (int32 j = i + 1; j < TeamIds.Num(); ++j)
        {
            FTournamentMatch Match;
            Match.MatchId = MatchId++;
            Match.TeamAId = TeamIds[i];
            Match.TeamBId = TeamIds[j];
            Match.Round = CurrentRound;
            Matches.Add(Match);
        }
    }
}

void USoccerTournamentManager::AdvanceKnockoutRound()
{
    TArray<int32> Winners;
    for (const FTournamentMatch& Match : Matches)
    {
        if (Match.Round == CurrentRound && Match.bCompleted && Match.WinnerTeamId > 0)
        {
            Winners.Add(Match.WinnerTeamId);
        }
    }

    if (Winners.Num() <= 1)
    {
        bIsActive = false;
        TournamentWinnerId = Winners.Num() == 1 ? Winners[0] : -1;
        return;
    }

    CurrentRound++;
    CreateKnockoutBracket(Winners);
    bIsActive = true;
}

bool USoccerTournamentManager::AreAllMatchesInRoundComplete(int32 Round) const
{
    for (const FTournamentMatch& Match : Matches)
    {
        if (Match.Round == Round && !Match.bCompleted)
        {
            return false;
        }
    }
    return true;
}

int32 USoccerTournamentManager::GetNextMatchId() const
{
    int32 HighestId = 0;
    for (const FTournamentMatch& Match : Matches)
    {
        HighestId = FMath::Max(HighestId, Match.MatchId);
    }
    return HighestId + 1;
}
