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

	// Physics settings
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Physics")
	float MaxBallSpeed;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Physics")
	float AirResistance;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Soccer|Physics")
	float KickPower;

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
