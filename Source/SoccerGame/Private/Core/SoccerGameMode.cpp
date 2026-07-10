// Copyright Frank Dev Code. All Rights Reserved.

#include "Core/SoccerGameMode.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerController.h"
#include "Game/SoccerGameState.h"
#include "Tools/SoccerMatchManager.h"
#include "Tools/SoccerTeamManager.h"
#include "Tools/SoccerTournamentManager.h"
#include "Characters/SoccerPlayerCharacter.h"
#include "Ball/SoccerBall.h"
#include "Kismet/GameplayStatics.h"
#include "AI/SimpleAIController.h"
#include "Tools/SoccerCareerManager.h"
#include "Tools/SoccerTrainingManager.h"
#include "Tools/SoccerReplayManager.h"
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

	USoccerReplayManager* ReplayManager = USoccerReplayManager::Get();
	if (ReplayManager && ReplayManager->IsReplaying())
	{
		ReplayManager->TickReplay(DeltaTime);
		return;
	}

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

	if (ReplayManager && ReplayManager->IsRecording())
	{
		TArray<AActor*> BallActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoccerBall::StaticClass(), BallActors);
		ASoccerBall* Ball = BallActors.Num() > 0 ? Cast<ASoccerBall>(BallActors[0]) : nullptr;

		if (ASoccerGameState* GameState = GetGameState<ASoccerGameState>())
		{
			ReplayManager->RecordFrame(DeltaTime, GameState->GetRegisteredPlayers(), Ball);
		}
	}

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

void ASoccerGameMode::StartQuickMatch(int32 TeamSize, float Duration, bool bAutoStart)
{
	PlayersPerTeam = TeamSize;
	InitializeMatch(TeamSize, Duration);

	UWorld* World = GetWorld();
	if (!World) return;

	UClass* PawnClass = DefaultPawnClass ? DefaultPawnClass : nullptr;

	// Fallback spawn points if none provided
	if (Team1SpawnPoints.Num() == 0)
	{
		Team1SpawnPoints.Add(FVector(-800.0f, -300.0f, 100.0f));
		Team1SpawnPoints.Add(FVector(-600.0f, -100.0f, 100.0f));
		Team1SpawnPoints.Add(FVector(-600.0f, 100.0f, 100.0f));
		Team1SpawnPoints.Add(FVector(-400.0f, -200.0f, 100.0f));
		Team1SpawnPoints.Add(FVector(-400.0f, 200.0f, 100.0f));
	}

	if (Team2SpawnPoints.Num() == 0)
	{
		// Mirror positions for team 2
		for (const FVector& V : Team1SpawnPoints)
		{
			Team2SpawnPoints.Add(FVector(-V.X, -V.Y, V.Z));
		}
	}

	// Spawn players for both teams
	for (int32 Team = 1; Team <= 2; ++Team)
	{
		const TArray<FVector>& Spawns = (Team == 1) ? Team1SpawnPoints : Team2SpawnPoints;

		for (int32 i = 0; i < TeamSize; ++i)
		{
			const FVector SpawnLoc = Spawns.IsValidIndex(i) ? Spawns[i] : Spawns[i % Spawns.Num()];
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			APawn* Pawn = nullptr;
			if (PawnClass)
			{
				Pawn = World->SpawnActor<APawn>(PawnClass, SpawnLoc, FRotator::ZeroRotator, Params);
			}

			if (!Pawn) continue;

			// Ensure AI possesses the pawn
			Pawn->SpawnDefaultController();

			ASoccerPlayerCharacter* PlayerChar = Cast<ASoccerPlayerCharacter>(Pawn);
			if (PlayerChar)
			{
				// Assign logical role: 0 -> GK, others distribution
				EPlayerPosition Role = EPlayerPosition::Midfielder;
				if (i == 0) Role = EPlayerPosition::Goalkeeper;
				else if (i <= 2) Role = EPlayerPosition::Defender;
				else if (i == 3) Role = EPlayerPosition::Midfielder;
				else Role = EPlayerPosition::Forward;

				PlayerChar->InitializePlayer(Team, Role, i + 1);
			}
		}
	}

	if (bAutoStart)
	{
		StartMatch();
	}
}

void ASoccerGameMode::StartCareerMode(const FString& PlayerName, bool bAutoStartMatch)
{
	USoccerCareerManager* Career = USoccerCareerManager::Get();
	if (!Career) return;

	Career->StartCareer(PlayerName);

	// Optionally start a quick match to jump into gameplay with the created profile
	if (bAutoStartMatch)
	{
		StartQuickMatch(PlayersPerTeam, MatchDurationSeconds, true);
	}
}

void ASoccerGameMode::StartTrainingMode(ETrainingDrillType DrillType)
{
	USoccerTrainingManager* TrainingManager = USoccerTrainingManager::Get();
	if (!TrainingManager)
	{
		return;
	}

	TrainingManager->StartTrainingSession(DrillType);

	// The training mode currently uses a reduced solo match.
	// It sets up the field, spawns a single controlled player, and provides a training drill.
	InitializeMatch(1, 300.0f);

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UClass* PawnClass = DefaultPawnClass ? DefaultPawnClass : nullptr;

	// Spawn a single training player
	if (PawnClass)
	{
		FVector TrainingSpawn(0.0f, -400.0f, 100.0f);
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		APawn* Pawn = World->SpawnActor<APawn>(PawnClass, TrainingSpawn, FRotator::ZeroRotator, Params);
		if (Pawn)
		{
			Pawn->SpawnDefaultController();
			ASoccerPlayerCharacter* PlayerChar = Cast<ASoccerPlayerCharacter>(Pawn);
			if (PlayerChar)
			{
				PlayerChar->InitializePlayer(1, EPlayerPosition::Forward, 1);
				PlayerChar->SetFormationTarget(FVector(0.0f, 0.0f, 0.0f));
			}
		}
	}

	// Optionally spawn a training ball at a fixed location
	TArray<AActor*> BallActors;
	UGameplayStatics::GetAllActorsOfClass(World, ASoccerBall::StaticClass(), BallActors);
	if (BallActors.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Training] No ball present for training session."));
	}

	StartMatch();
}

void ASoccerGameMode::EndTrainingMode()
{
	if (USoccerTrainingManager* TrainingManager = USoccerTrainingManager::Get())
	{
		TrainingManager->ResetTrainingSession();
	}

	EndMatch(-1);
}

void ASoccerGameMode::StartTournamentMode(EGameTournamentType TournamentType, const TArray<int32>& TeamIds)
{
	USoccerTournamentManager* TournamentManager = USoccerTournamentManager::Get();
	if (!TournamentManager)
	{
		return;
	}

	TArray<int32> TournamentTeams = TeamIds;
	if (TournamentTeams.Num() < 2)
	{
		USoccerTeamManager* TeamManager = USoccerTeamManager::Get();
		if (TeamManager)
		{
			for (const FSoccerTeamRecord& Team : TeamManager->GetTeams())
			{
				TournamentTeams.Add(Team.TeamId);
				if (TournamentTeams.Num() >= 4)
				{
					break;
				}
			}
		}
	}

	if (TournamentTeams.Num() < 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Tournament] Not enough teams configured for tournament."));
		return;
	}

	if (!TournamentManager->InitializeTournament(TournamentType, TournamentTeams))
	{
		UE_LOG(LogTemp, Warning, TEXT("[Tournament] Failed to initialize tournament."));
		return;
	}

	TournamentManager->StartTournament();
	InitializeMatch(PlayersPerTeam, MatchDurationSeconds);

	UE_LOG(LogTemp, Warning, TEXT("[SoccerGameMode] Tournament started with %d teams."), TournamentTeams.Num());
}

void ASoccerGameMode::EndTournamentMode()
{
	if (USoccerTournamentManager* TournamentManager = USoccerTournamentManager::Get())
	{
		TournamentManager->ResetTournament();
	}

	EndMatch(-1);
}

void ASoccerGameMode::StartReplayRecording(const FString& ReplayName)
{
	USoccerReplayManager* ReplayManager = USoccerReplayManager::Get();
	if (!ReplayManager)
	{
		return;
	}

	ReplayManager->StartRecording(ReplayName);
}

void ASoccerGameMode::StopReplayRecording()
{
	if (USoccerReplayManager* ReplayManager = USoccerReplayManager::Get())
	{
		ReplayManager->StopRecording();
	}
}

bool ASoccerGameMode::StartReplayPlayback()
{
	USoccerReplayManager* ReplayManager = USoccerReplayManager::Get();
	if (!ReplayManager)
	{
		return false;
	}

	return ReplayManager->StartReplay();
}

void ASoccerGameMode::StopReplayPlayback()
{
	if (USoccerReplayManager* ReplayManager = USoccerReplayManager::Get())
	{
		ReplayManager->StopReplay();
	}
}
