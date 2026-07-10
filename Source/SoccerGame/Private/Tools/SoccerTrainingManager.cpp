// Copyright Frank Dev Code. All Rights Reserved.

#include "Tools/SoccerTrainingManager.h"
#include "Misc/Guid.h"

USoccerTrainingManager* USoccerTrainingManager::Instance = nullptr;

USoccerTrainingManager* USoccerTrainingManager::Get()
{
	if (!Instance)
	{
		Instance = NewObject<USoccerTrainingManager>();
		Instance->AddToRoot();
	}
	return Instance;
}

void USoccerTrainingManager::StartTrainingSession(ETrainingDrillType DrillType)
{
	CurrentDrill.DrillType = DrillType;
	CurrentDrill.DrillName = FName(*FString::Printf(TEXT("Drill_%d"), static_cast<int32>(DrillType)));
	CurrentDrill.TargetScore = 5;
	CurrentDrill.bIsCompleted = false;

	UE_LOG(LogTemp, Warning, TEXT("[Training] Started training session: %s"), *CurrentDrill.DrillName.ToString());
}

FTrainingDrill USoccerTrainingManager::GetCurrentDrill() const
{
	return CurrentDrill;
}

void USoccerTrainingManager::CompleteDrill()
{
	CurrentDrill.bIsCompleted = true;
	UE_LOG(LogTemp, Warning, TEXT("[Training] Completed training drill: %s"), *CurrentDrill.DrillName.ToString());
}

void USoccerTrainingManager::ResetTrainingSession()
{
	CurrentDrill = FTrainingDrill();
	UE_LOG(LogTemp, Warning, TEXT("[Training] Training session reset."));
}
