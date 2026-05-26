// Copyright Frank Dev Code. All Rights Reserved.

#include "Tools/SoccerMatchManager.h"

USoccerMatchManager* USoccerMatchManager::Instance = nullptr;

USoccerMatchManager* USoccerMatchManager::Get()
{
	if (!Instance)
	{
		Instance = NewObject<USoccerMatchManager>();
		Instance->AddToRoot();
	}

	return Instance;
}

void USoccerMatchManager::Initialize(float MatchDuration)
{
	CurrentSummary.Duration = FMath::Max(0.0f, MatchDuration);
	CurrentSummary.ElapsedTime = 0.0f;
	CurrentSummary.bIsActive = false;
	CurrentSummary.bIsPaused = false;
	CurrentSummary.WinnerTeamId = -1;
}

void USoccerMatchManager::Start()
{
	CurrentSummary.bIsActive = true;
	CurrentSummary.bIsPaused = false;
}

void USoccerMatchManager::Pause()
{
	CurrentSummary.bIsPaused = true;
}

void USoccerMatchManager::Resume()
{
	CurrentSummary.bIsPaused = false;
}

void USoccerMatchManager::End(int32 WinningTeamId)
{
	CurrentSummary.bIsActive = false;
	CurrentSummary.bIsPaused = false;
	CurrentSummary.WinnerTeamId = WinningTeamId;
}

void USoccerMatchManager::Tick(float DeltaTime)
{
	if (!CurrentSummary.bIsActive || CurrentSummary.bIsPaused)
	{
		return;
	}

	CurrentSummary.ElapsedTime = FMath::Min(CurrentSummary.ElapsedTime + DeltaTime, CurrentSummary.Duration);

	if (CurrentSummary.ElapsedTime >= CurrentSummary.Duration)
	{
		End(-1);
	}
}

FSoccerMatchSummary USoccerMatchManager::GetSummary() const
{
	return CurrentSummary;
}

float USoccerMatchManager::GetRemainingTime() const
{
	return FMath::Max(0.0f, CurrentSummary.Duration - CurrentSummary.ElapsedTime);
}

void USoccerMatchManager::Reset()
{
	CurrentSummary = FSoccerMatchSummary();
}
