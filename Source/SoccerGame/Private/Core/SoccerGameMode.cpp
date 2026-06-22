// Copyright Frank Dev Code. All Rights Reserved.

#include "Core/SoccerGameMode.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerController.h"
#include "Game/SoccerGameState.h"
#include "Tools/SoccerMatchManager.h"
#include "Tools/SoccerTeamManager.h"
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

	InitializeMatch(PlayersPerTeam, MatchDurationSeconds);

	UE_LOG(LogTemp, Warning, TEXT("[SoccerGameMode] Game mode initialized"));
}

void ASoccerGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bMatchActive || bMatchPaused)
	{
		return;
	}

	USoccerMatchManager* MatchManager = USoccerMatchManager::Get();
	if (!MatchManager)
	{
		return;
	}

	MatchManager->Tick(DeltaTime);
	const FSoccerMatchSummary MatchSummary = MatchManager->GetSummary();
	MatchElapsedTime = MatchSummary.ElapsedTime;

	if (!MatchSummary.bIsActive)
	{
		EndMatch(MatchSummary.WinnerTeamId);
		return;
	}

	CheckMatchConditions();

	if (ASoccerGameState* GameState = GetGameState<ASoccerGameState>())
	{
		GameState->SetMatchActive(true);
		GameState->SetMatchElapsedTime(MatchElapsedTime);
	}
}

void ASoccerGameMode::InitializeMatch(int32 PlayerCount, float MatchDurationSeconds)
{
	PlayersPerTeam = PlayerCount;
	this->MatchDurationSeconds = MatchDurationSeconds;
	MatchElapsedTime = 0.0f;
	bMatchActive = false;
	bMatchPaused = false;

	USoccerMatchManager* MatchManager = USoccerMatchManager::Get();
	if (MatchManager)
	{
		MatchManager->Initialize(this->MatchDurationSeconds);
		MatchManager->Reset();
		MatchManager->Initialize(this->MatchDurationSeconds);
	}

	USoccerTeamManager* TeamManager = USoccerTeamManager::Get();
	if (TeamManager)
	{
		TeamManager->RegisterTeam(FSoccerTeamRecord{1, TEXT("Team 1"), 0, 0, 0});
		TeamManager->RegisterTeam(FSoccerTeamRecord{2, TEXT("Team 2"), 0, 0, 0});
	}

	// Apply default formations on server start
	USoccerFormationManager* FormationManager = USoccerFormationManager::Get();
	if (FormationManager)
	{
		FormationManager->ApplyDefaultFormation(1);
		FormationManager->ApplyDefaultFormation(2);
	}

	if (ASoccerGameState* GameState = GetGameState<ASoccerGameState>())
	{
		GameState->SetMatchActive(false);
		GameState->SetMatchElapsedTime(0.0f);
	}

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

	if (USoccerMatchManager* MatchManager = USoccerMatchManager::Get())
	{
		MatchManager->Initialize(MatchDurationSeconds);
		MatchManager->Start();
	}

	if (ASoccerGameState* GameState = GetGameState<ASoccerGameState>())
	{
		GameState->SetMatchActive(true);
		GameState->SetMatchElapsedTime(0.0f);
	}

	UE_LOG(LogTemp, Warning, TEXT("[SoccerGameMode] Match started"));
}

void ASoccerGameMode::EndMatch(int32 WinningTeamId)
{
	if (!bMatchActive)
	{
		return;
	}

	bMatchActive = false;
	bMatchPaused = false;
	MatchElapsedTime = MatchDurationSeconds;

	if (USoccerMatchManager* MatchManager = USoccerMatchManager::Get())
	{
		MatchManager->End(WinningTeamId);
	}

	if (ASoccerGameState* GameState = GetGameState<ASoccerGameState>())
	{
		GameState->SetMatchActive(false);
		GameState->SetMatchElapsedTime(MatchDurationSeconds);
	}

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
	if (USoccerMatchManager* MatchManager = USoccerMatchManager::Get())
	{
		MatchManager->Pause();
	}
	UE_LOG(LogTemp, Warning, TEXT("[SoccerGameMode] Match paused"));
}

void ASoccerGameMode::ResumeMatch()
{
	if (!bMatchActive || !bMatchPaused)
	{
		return;
	}

	bMatchPaused = false;
	if (USoccerMatchManager* MatchManager = USoccerMatchManager::Get())
	{
		MatchManager->Resume();
	}
	UE_LOG(LogTemp, Warning, TEXT("[SoccerGameMode] Match resumed"));
}

void ASoccerGameMode::UpdateMatchTime(float DeltaTime)
{
	if (USoccerMatchManager* MatchManager = USoccerMatchManager::Get())
	{
		MatchManager->Tick(DeltaTime);
		const FSoccerMatchSummary MatchSummary = MatchManager->GetSummary();
		MatchElapsedTime = MatchSummary.ElapsedTime;
	}
}

void ASoccerGameMode::CheckMatchConditions()
{
	// TODO: Implement match condition checks (goals, fouls, etc.)
}
