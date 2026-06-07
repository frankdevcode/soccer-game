// Copyright Frank Dev Code. All Rights Reserved.

#include "Core/SoccerGameSettings.h"

USoccerGameSettings* USoccerGameSettings::Instance = nullptr;

USoccerGameSettings::USoccerGameSettings()
	: MatchDuration(1200.0f) // 20 minutes
	, PlayersPerTeam(5)
	, BallRadius(11.0f) // cm
	, BallMass(0.43f) // kg
	, FieldLength(4000.0f) // 40m
	, FieldWidth(2000.0f) // 20m
	, GoalWidth(700.0f)
	, GoalDepth(200.0f)
	, BoundaryHeight(120.0f)
	, BoundaryThickness(30.0f)
	, CameraFollowDistance(400.0f)
	, CameraMinDistance(250.0f)
	, CameraMaxDistance(800.0f)
	, CameraZoomStep(50.0f)
	, CameraPitchMin(-60.0f)
	, CameraPitchMax(20.0f)
	, CameraLagSpeed(12.0f)
	, CameraRotationLagSpeed(12.0f)
	, MaxBallSpeed(5000.0f) // cm/s
	, AirResistance(0.1f)
	, KickPower(1000.0f)
	, BackendURL(TEXT("localhost"))
	, BackendPort(3001)
	, NetworkTickRate(60.0f)
{
	SetDefaults();
}

USoccerGameSettings* USoccerGameSettings::Get()
{
	if (!Instance)
	{
		Instance = NewObject<USoccerGameSettings>();
		Instance->AddToRoot();
		Instance->LoadSettings();
	}
	return Instance;
}

void USoccerGameSettings::SaveSettings()
{
	UpdateDefaultConfigFile();
	UE_LOG(LogTemp, Warning, TEXT("[SoccerGameSettings] Settings saved"));
}

void USoccerGameSettings::LoadSettings()
{
	LoadConfig();
	UE_LOG(LogTemp, Warning, TEXT("[SoccerGameSettings] Settings loaded"));
}

void USoccerGameSettings::ResetToDefaults()
{
	SetDefaults();
	UE_LOG(LogTemp, Warning, TEXT("[SoccerGameSettings] Settings reset to defaults"));
}

void USoccerGameSettings::SetDefaults()
{
	MatchDuration = 1200.0f;
	PlayersPerTeam = 5;
	BallRadius = 11.0f;
	BallMass = 0.43f;
	FieldLength = 4000.0f;
	FieldWidth = 2000.0f;
	GoalWidth = 700.0f;
	GoalDepth = 200.0f;
	BoundaryHeight = 120.0f;
	BoundaryThickness = 30.0f;
	CameraFollowDistance = 400.0f;
	CameraMinDistance = 250.0f;
	CameraMaxDistance = 800.0f;
	CameraZoomStep = 50.0f;
	CameraPitchMin = -60.0f;
	CameraPitchMax = 20.0f;
	CameraLagSpeed = 12.0f;
	CameraRotationLagSpeed = 12.0f;
	MaxBallSpeed = 5000.0f;
	AirResistance = 0.1f;
	KickPower = 1000.0f;
	BackendURL = TEXT("localhost");
	BackendPort = 3001;
	NetworkTickRate = 60.0f;
}
