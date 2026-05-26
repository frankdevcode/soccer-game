// Copyright Frank Dev Code. All Rights Reserved.

#include "Core/SoccerGameMode.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerController.h"
#include "Game/SoccerGameState.h"
#include "UObject/ConstructorHelpers.h"

ASoccerGameMode::ASoccerGameMode()
	: bMatchActive(false)
	, bMatchPaused(false)
	, MatchElapsedTime(0.0f)
	, MatchDurationSeconds(1200.0f) // 20 minutes
	, PlayersPerTeam(5)
{
	// Set default pawn class
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
		TEXT("/Game/Blueprints/Characters/BP_PlayerCharacter"));
	
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// Enable tick
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;
}

void ASoccerGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("[SoccerGameMode] Game mode initialized"));
}

void ASoccerGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMatchActive && !bMatchPaused)
	{
		UpdateMatchTime(DeltaTime);
		CheckMatchConditions();
	}
}

void ASoccerGameMode::InitializeMatch(int32 PlayerCount, float MatchDurationSeconds)
{
	PlayersPerTeam = PlayerCount;
	this->MatchDurationSeconds = MatchDurationSeconds;
	MatchElapsedTime = 0.0f;
	bMatchActive = false;
	bMatchPaused = false;

	UE_LOG(LogTemp, Warning, TEXT("[SoccerGameMode] Match initialized - %d vs %d, Duration: %.0f seconds"),
		PlayersPerTeam, PlayersPerTeam, MatchDurationSeconds);
}

void ASoccerGameMode::StartMatch()
{
	if (bMatchActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SoccerGameMode] Match already started"));
		return;
	}

	bMatchActive = true;
	bMatchPaused = false;
	MatchElapsedTime = 0.0f;

	UE_LOG(LogTemp, Warning, TEXT("[SoccerGameMode] Match started"));
}

void ASoccerGameMode::EndMatch(int32 WinningTeamId)
{
	if (!bMatchActive)
	{
		return;
	}

	bMatchActive = false;
	MatchElapsedTime = MatchDurationSeconds;

	if (WinningTeamId == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SoccerGameMode] Match ended - Draw"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[SoccerGameMode] Match ended - Team %d won"), WinningTeamId);
	}
}

void ASoccerGameMode::PauseMatch()
{
	if (!bMatchActive || bMatchPaused)
	{
		return;
	}

	bMatchPaused = true;
	UE_LOG(LogTemp, Warning, TEXT("[SoccerGameMode] Match paused"));
}

void ASoccerGameMode::ResumeMatch()
{
	if (!bMatchActive || !bMatchPaused)
	{
		return;
	}

	bMatchPaused = false;
	UE_LOG(LogTemp, Warning, TEXT("[SoccerGameMode] Match resumed"));
}

void ASoccerGameMode::UpdateMatchTime(float DeltaTime)
{
	MatchElapsedTime += DeltaTime;

	if (MatchElapsedTime >= MatchDurationSeconds)
	{
		EndMatch(-1); // Draw on time expiration
	}
}

void ASoccerGameMode::CheckMatchConditions()
{
	// TODO: Implement match condition checks (goals, fouls, etc.)
}
