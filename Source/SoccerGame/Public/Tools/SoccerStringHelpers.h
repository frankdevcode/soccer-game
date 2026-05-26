// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoccerStringHelpers.generated.h"

/**
 * @class USoccerStringHelpers
 * @brief Utility helpers for normalizing and formatting strings.
 */
UCLASS()
class SOCCERGAME_API USoccerStringHelpers : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	static FString Sanitize(const FString& Input);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	static FString ToTitleCase(const FString& Input);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	static FString FormatPlayerLabel(int32 TeamId, int32 PlayerNumber, const FString& Position);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	static bool IsNullOrEmpty(const FString& Input);
};
