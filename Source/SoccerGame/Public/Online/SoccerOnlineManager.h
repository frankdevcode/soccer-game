// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoccerOnlineManager.generated.h"

UENUM(BlueprintType)
enum class ESoccerMatchmakingState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Searching UMETA(DisplayName = "Searching"),
	Matched UMETA(DisplayName = "Matched"),
	Connected UMETA(DisplayName = "Connected")
};

USTRUCT(BlueprintType)
struct FSoccerOnlinePlayerProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 XP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Wins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Losses;
};

UCLASS(BlueprintType)
class SOCCERGAME_API USoccerOnlineManager : public UObject
{
	GENERATED_BODY()

public:
	USoccerOnlineManager();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Online")
	void Initialize();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Online")
	void StartMatchmaking(const FString& PlayerName);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Online")
	void StopMatchmaking();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Online")
	void ConnectToMatch();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Online")
	void SyncGameState(const FString& MatchId, float MatchTime, int32 TeamAScore, int32 TeamBScore);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Online")
	void AwardXP(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Online")
	FSoccerOnlinePlayerProfile GetLocalProfile() const { return LocalProfile; }

	UFUNCTION(BlueprintCallable, Category = "Soccer|Online")
	ESoccerMatchmakingState GetMatchmakingState() const { return MatchmakingState; }

	UFUNCTION(BlueprintCallable, Category = "Soccer|Online")
	FString GetActiveMatchId() const { return ActiveMatchId; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Online")
	FSoccerOnlinePlayerProfile LocalProfile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Online")
	ESoccerMatchmakingState MatchmakingState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Online")
	FString ActiveMatchId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Online")
	bool bConnected;
};
