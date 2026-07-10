// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoccerTournamentManager.generated.h"

UENUM(BlueprintType)
enum class EGameTournamentType : uint8
{
    Knockout UMETA(DisplayName = "Knockout"),
    League UMETA(DisplayName = "League")
};

USTRUCT(BlueprintType)
struct FTournamentMatch
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Tournament")
    int32 MatchId;

    UPROPERTY(BlueprintReadWrite, Category = "Tournament")
    int32 TeamAId;

    UPROPERTY(BlueprintReadWrite, Category = "Tournament")
    int32 TeamBId;

    UPROPERTY(BlueprintReadWrite, Category = "Tournament")
    int32 TeamAScore;

    UPROPERTY(BlueprintReadWrite, Category = "Tournament")
    int32 TeamBScore;

    UPROPERTY(BlueprintReadWrite, Category = "Tournament")
    int32 WinnerTeamId;

    UPROPERTY(BlueprintReadWrite, Category = "Tournament")
    int32 Round;

    UPROPERTY(BlueprintReadWrite, Category = "Tournament")
    bool bCompleted;

    FTournamentMatch()
        : MatchId(-1)
        , TeamAId(0)
        , TeamBId(0)
        , TeamAScore(0)
        , TeamBScore(0)
        , WinnerTeamId(-1)
        , Round(0)
        , bCompleted(false)
    {
    }
};

USTRUCT(BlueprintType)
struct FTournamentSummary
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Tournament")
    EGameTournamentType TournamentType;

    UPROPERTY(BlueprintReadWrite, Category = "Tournament")
    int32 TotalTeams;

    UPROPERTY(BlueprintReadWrite, Category = "Tournament")
    int32 CurrentRound;

    UPROPERTY(BlueprintReadWrite, Category = "Tournament")
    bool bIsActive;

    UPROPERTY(BlueprintReadWrite, Category = "Tournament")
    int32 TournamentWinnerId;

    FTournamentSummary()
        : TournamentType(EGameTournamentType::Knockout)
        , TotalTeams(0)
        , CurrentRound(0)
        , bIsActive(false)
        , TournamentWinnerId(-1)
    {
    }
};

UCLASS()
class SOCCERGAME_API USoccerTournamentManager : public UObject
{
    GENERATED_BODY()

public:
    static USoccerTournamentManager* Get();

    UFUNCTION(BlueprintCallable, Category = "Tournament")
    bool InitializeTournament(EGameTournamentType TournamentType, const TArray<int32>& TeamIds);

    UFUNCTION(BlueprintCallable, Category = "Tournament")
    bool StartTournament();

    UFUNCTION(BlueprintCallable, Category = "Tournament")
    bool RecordMatchResult(int32 MatchId, int32 TeamAScore, int32 TeamBScore);

    UFUNCTION(BlueprintCallable, Category = "Tournament")
    TArray<FTournamentMatch> GetMatches() const;

    UFUNCTION(BlueprintCallable, Category = "Tournament")
    FTournamentSummary GetSummary() const;

    UFUNCTION(BlueprintCallable, Category = "Tournament")
    bool IsTournamentComplete() const;

    UFUNCTION(BlueprintCallable, Category = "Tournament")
    void ResetTournament();

private:
    void CreateInitialBracket();
    void CreateKnockoutBracket(const TArray<int32>& TeamIds);
    void CreateLeagueBracket(const TArray<int32>& TeamIds);
    void AdvanceKnockoutRound();
    bool AreAllMatchesInRoundComplete(int32 Round) const;
    int32 GetNextMatchId() const;

    static USoccerTournamentManager* Instance;

    EGameTournamentType TournamentType;
    TArray<int32> RegisteredTeams;
    TArray<FTournamentMatch> Matches;
    int32 CurrentRound;
    bool bIsActive;
    int32 TournamentWinnerId;
};
