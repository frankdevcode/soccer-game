// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoccerGameLogging.generated.h"

/**
 * @class USoccerGameLogging
 * @brief Centralized logging system for Soccer-Game
 * 
 * Provides structured logging with different severity levels.
 * Used throughout the codebase for consistent logging.
 */
UCLASS()
class SOCCERGAME_API USoccerGameLogging : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * @brief Log an informational message
	 * @param Category Log category (e.g., "Gameplay", "Network")
	 * @param Message Message to log
	 */
	UFUNCTION(BlueprintCallable, Category = "Soccer|Logging")
	static void LogInfo(const FString& Category, const FString& Message);

	/**
	 * @brief Log a warning message
	 * @param Category Log category
	 * @param Message Message to log
	 */
	UFUNCTION(BlueprintCallable, Category = "Soccer|Logging")
	static void LogWarning(const FString& Category, const FString& Message);

	/**
	 * @brief Log an error message
	 * @param Category Log category
	 * @param Message Message to log
	 */
	UFUNCTION(BlueprintCallable, Category = "Soccer|Logging")
	static void LogError(const FString& Category, const FString& Message);

	/**
	 * @brief Log a critical message
	 * @param Category Log category
	 * @param Message Message to log
	 */
	UFUNCTION(BlueprintCallable, Category = "Soccer|Logging")
	static void LogCritical(const FString& Category, const FString& Message);
};
