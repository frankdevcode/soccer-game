// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoccerPlayerManager.generated.h"

/**
 * @struct FSoccerPlayerRecord
 * @brief Runtime player data used by project tools and managers.
 */
USTRUCT(BlueprintType)
struct FSoccerPlayerRecord
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Players")
	FString PlayerId;

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Players")
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Players")
	int32 TeamId;

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Players")
	int32 PlayerNumber;

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Players")
	float Stamina;

	UPROPERTY(BlueprintReadWrite, Category = "Soccer|Players")
	bool bIsActive;

	FSoccerPlayerRecord()
		: TeamId(0)
		, PlayerNumber(0)
		, Stamina(100.0f)
		, bIsActive(false)
	{
	}
};

/**
 * @class USoccerPlayerManager
 * @brief Maintains player runtime records and roster lookups.
 */
UCLASS()
class SOCCERGAME_API USoccerPlayerManager : public UObject
{
	GENERATED_BODY()

public:
	static USoccerPlayerManager* Get();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	bool RegisterPlayer(const FSoccerPlayerRecord& PlayerRecord);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	bool UpdatePlayer(const FSoccerPlayerRecord& PlayerRecord);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	bool UnregisterPlayer(const FString& PlayerId);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	FSoccerPlayerRecord GetPlayer(const FString& PlayerId) const;

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	TArray<FSoccerPlayerRecord> GetPlayersForTeam(int32 TeamId) const;

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	int32 GetPlayerCount() const;

	UFUNCTION(BlueprintCallable, Category = "Soccer|Utilities")
	void Clear();

private:
	static USoccerPlayerManager* Instance;

	TMap<FString, FSoccerPlayerRecord> Players;
};
