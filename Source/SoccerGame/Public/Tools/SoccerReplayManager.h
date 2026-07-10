// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoccerReplayManager.generated.h"

class ASoccerBall;
class ASoccerPlayerCharacter;

USTRUCT(BlueprintType)
struct FReplayPlayerState
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Replay")
    int32 TeamId;

    UPROPERTY(BlueprintReadWrite, Category = "Replay")
    int32 PlayerNumber;

    UPROPERTY(BlueprintReadWrite, Category = "Replay")
    FVector Location;

    UPROPERTY(BlueprintReadWrite, Category = "Replay")
    FRotator Rotation;

    UPROPERTY(BlueprintReadWrite, Category = "Replay")
    FVector Velocity;

    FReplayPlayerState()
        : TeamId(0)
        , PlayerNumber(0)
        , Location(FVector::ZeroVector)
        , Rotation(FRotator::ZeroRotator)
        , Velocity(FVector::ZeroVector)
    {
    }
};

USTRUCT(BlueprintType)
struct FReplayBallState
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Replay")
    FVector Location;

    UPROPERTY(BlueprintReadWrite, Category = "Replay")
    FRotator Rotation;

    UPROPERTY(BlueprintReadWrite, Category = "Replay")
    FVector Velocity;

    FReplayBallState()
        : Location(FVector::ZeroVector)
        , Rotation(FRotator::ZeroRotator)
        , Velocity(FVector::ZeroVector)
    {
    }
};

USTRUCT(BlueprintType)
struct FReplayFrame
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Replay")
    float TimeStamp;

    UPROPERTY(BlueprintReadWrite, Category = "Replay")
    TArray<FReplayPlayerState> PlayerStates;

    UPROPERTY(BlueprintReadWrite, Category = "Replay")
    FReplayBallState BallState;

    FReplayFrame()
        : TimeStamp(0.0f)
    {
    }
};

USTRUCT(BlueprintType)
struct FMatchReplay
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Replay")
    FString ReplayName;

    UPROPERTY(BlueprintReadWrite, Category = "Replay")
    float Duration;

    UPROPERTY(BlueprintReadWrite, Category = "Replay")
    TArray<FReplayFrame> Frames;

    FMatchReplay()
        : ReplayName(TEXT("DefaultReplay"))
        , Duration(0.0f)
    {
    }
};

UCLASS()
class SOCCERGAME_API USoccerReplayManager : public UObject
{
    GENERATED_BODY()

public:
    static USoccerReplayManager* Get();

    UFUNCTION(BlueprintCallable, Category = "Replay")
    void StartRecording(const FString& ReplayName = TEXT("MatchReplay"));

    UFUNCTION(BlueprintCallable, Category = "Replay")
    void RecordFrame(float DeltaTime, const TArray<ASoccerPlayerCharacter*>& Players, ASoccerBall* Ball);

    UFUNCTION(BlueprintCallable, Category = "Replay")
    void StopRecording();

    UFUNCTION(BlueprintCallable, Category = "Replay")
    bool StartReplay();

    UFUNCTION(BlueprintCallable, Category = "Replay")
    void TickReplay(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Replay")
    void StopReplay();

    UFUNCTION(BlueprintCallable, Category = "Replay")
    bool IsRecording() const;

    UFUNCTION(BlueprintCallable, Category = "Replay")
    bool IsReplaying() const;

    UFUNCTION(BlueprintCallable, Category = "Replay")
    FMatchReplay GetCurrentReplay() const;

    UFUNCTION(BlueprintCallable, Category = "Replay")
    void ResetReplay();

private:
    static USoccerReplayManager* Instance;

    bool bIsRecording;
    bool bIsReplaying;
    float ReplayTime;
    int32 CurrentFrameIndex;
    FMatchReplay CurrentReplay;

    void ApplyFrame(const FReplayFrame& Frame);
    ASoccerPlayerCharacter* FindReplayPlayer(const FReplayPlayerState& PlayerState) const;
    ASoccerBall* FindReplayBall() const;
};
