// Copyright Frank Dev Code. All Rights Reserved.

#include "Core/SoccerGameLogging.h"

void USoccerGameLogging::LogInfo(const FString& Category, const FString& Message)
{
	FString LogMessage = FString::Printf(TEXT("[%s] %s"), *Category, *Message);
	UE_LOG(LogTemp, Log, TEXT("%s"), *LogMessage);
}

void USoccerGameLogging::LogWarning(const FString& Category, const FString& Message)
{
	FString LogMessage = FString::Printf(TEXT("[%s] %s"), *Category, *Message);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMessage);
}

void USoccerGameLogging::LogError(const FString& Category, const FString& Message)
{
	FString LogMessage = FString::Printf(TEXT("[%s] %s"), *Category, *Message);
	UE_LOG(LogTemp, Error, TEXT("%s"), *LogMessage);
}

void USoccerGameLogging::LogCritical(const FString& Category, const FString& Message)
{
	FString LogMessage = FString::Printf(TEXT("[%s] CRITICAL - %s"), *Category, *Message);
	UE_LOG(LogTemp, Fatal, TEXT("%s"), *LogMessage);
}
