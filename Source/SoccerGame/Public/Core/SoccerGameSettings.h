// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoccerGameSettings.generated.h"

/**
 * @class USoccerGameSettings
 * @brief Global game settings and configuration
 * 
 * Centralized configuration for game parameters.
 * Can be loaded from files and modified at runtime.
 */
UCLASS(Config = Game)
class SOCCERGAME_API USoccerGameSettings : public UObject
{
	GENERATED_BODY()

public:
	USoccerGameSettings();

	// Gameplay settings
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Gameplay")
	float MatchDuration;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Gameplay")
	int32 PlayersPerTeam;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Gameplay")
	float BallRadius;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Gameplay")
	float BallMass;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Gameplay")
	float FieldLength;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Gameplay")
	float FieldWidth;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Gameplay")
	float GoalWidth;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Gameplay")
	float GoalDepth;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Gameplay")
	float BoundaryHeight;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Gameplay")
	float BoundaryThickness;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Camera")
	float CameraFollowDistance;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Camera")
	float CameraMinDistance;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Camera")
	float CameraMaxDistance;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Camera")
	float CameraZoomStep;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Camera")
	float CameraPitchMin;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Camera")
	float CameraPitchMax;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Camera")
	float CameraLagSpeed;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Camera")
	float CameraRotationLagSpeed;

	// Physics settings
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Physics")
	float MaxBallSpeed;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Physics")
	float AirResistance;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Physics")
	float BallMagnusCoefficient;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Physics")
	float BallGroundFriction;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Physics")
	float BallSpinDamping;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Physics")
	float BallBounceCoefficient;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Physics")
	float BallCollisionFriction;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Physics")
	float FloorBounceCoefficient;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Physics")
	float FloorFriction;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Physics")
	float WallBounceCoefficient;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Physics")
	float WallFriction;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Physics")
	float KickPower;

	// Audio settings
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	float MasterVolume;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	float SFXVolume;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	float AmbientVolume;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	bool bAudioEnabled;

	// Network settings
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Network")
	FString BackendURL;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Network")
	int32 BackendPort;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Network")
	float NetworkTickRate;

	// Get singleton instance
	UFUNCTION(BlueprintCallable, Category = "Soccer|Settings")
	static USoccerGameSettings* Get();

	// Save and load settings
	UFUNCTION(BlueprintCallable, Category = "Soccer|Settings")
	void SaveSettings();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Settings")
	void LoadSettings();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Settings")
	void ResetToDefaults();

private:
	static USoccerGameSettings* Instance;

	void SetDefaults();
};
