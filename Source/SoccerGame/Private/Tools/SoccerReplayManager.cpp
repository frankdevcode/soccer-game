// Copyright Frank Dev Code. All Rights Reserved.

#include "Tools/SoccerReplayManager.h"
#include "Tools/SoccerMatchManager.h"
#include "Game/SoccerGameState.h"
#include "Characters/SoccerPlayerCharacter.h"
#include "Ball/SoccerBall.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

USoccerReplayManager* USoccerReplayManager::Instance = nullptr;

USoccerReplayManager* USoccerReplayManager::Get()
{
    if (!Instance)
    {
        Instance = NewObject<USoccerReplayManager>();
        Instance->AddToRoot();
    }
    return Instance;
}

void USoccerReplayManager::StartRecording(const FString& ReplayName)
{
    CurrentReplay = FMatchReplay();
    CurrentReplay.ReplayName = ReplayName;
    CurrentReplay.Duration = 0.0f;
    CurrentReplay.Frames.Empty();
    bIsRecording = true;
    bIsReplaying = false;
    ReplayTime = 0.0f;
    CurrentFrameIndex = 0;

    UE_LOG(LogTemp, Warning, TEXT("[Replay] Recording started: %s"), *ReplayName);
}

void USoccerReplayManager::RecordFrame(float DeltaTime, const TArray<ASoccerPlayerCharacter*>& Players, ASoccerBall* Ball)
{
    if (!bIsRecording)
    {
        return;
    }

    ReplayTime += DeltaTime;

    FReplayFrame Frame;
    Frame.TimeStamp = ReplayTime;

    for (ASoccerPlayerCharacter* Player : Players)
    {
        if (!Player)
        {
            continue;
        }

        FReplayPlayerState PlayerState;
        PlayerState.TeamId = Player->GetTeamId();
        PlayerState.PlayerNumber = Player->GetPlayerNumber();
        PlayerState.Location = Player->GetActorLocation();
        PlayerState.Rotation = Player->GetActorRotation();
        PlayerState.Velocity = Player->GetVelocity();
        Frame.PlayerStates.Add(PlayerState);
    }

    if (Ball)
    {
        Frame.BallState.Location = Ball->GetActorLocation();
        Frame.BallState.Rotation = Ball->GetActorRotation();
        Frame.BallState.Velocity = Ball->GetVelocity();
    }

    CurrentReplay.Frames.Add(Frame);
    CurrentReplay.Duration = ReplayTime;
}

void USoccerReplayManager::StopRecording()
{
    bIsRecording = false;
    UE_LOG(LogTemp, Warning, TEXT("[Replay] Recording stopped. Frames saved: %d"), CurrentReplay.Frames.Num());
}

bool USoccerReplayManager::StartReplay()
{
    if (CurrentReplay.Frames.Num() == 0)
    {
        return false;
    }

    bIsReplaying = true;
    bIsRecording = false;
    ReplayTime = 0.0f;
    CurrentFrameIndex = 0;

    UE_LOG(LogTemp, Warning, TEXT("[Replay] Replay started: %s"), *CurrentReplay.ReplayName);
    return true;
}

void USoccerReplayManager::TickReplay(float DeltaTime)
{
    if (!bIsReplaying || CurrentFrameIndex >= CurrentReplay.Frames.Num())
    {
        return;
    }

    ReplayTime += DeltaTime;
    const FReplayFrame& Frame = CurrentReplay.Frames[CurrentFrameIndex];
    if (ReplayTime >= Frame.TimeStamp)
    {
        ApplyFrame(Frame);
        CurrentFrameIndex++;

        if (CurrentFrameIndex >= CurrentReplay.Frames.Num())
        {
            StopReplay();
        }
    }
}

void USoccerReplayManager::StopReplay()
{
    bIsReplaying = false;
    UE_LOG(LogTemp, Warning, TEXT("[Replay] Replay ended."));
}

bool USoccerReplayManager::IsRecording() const
{
    return bIsRecording;
}

bool USoccerReplayManager::IsReplaying() const
{
    return bIsReplaying;
}

FMatchReplay USoccerReplayManager::GetCurrentReplay() const
{
    return CurrentReplay;
}

void USoccerReplayManager::ResetReplay()
{
    CurrentReplay = FMatchReplay();
    bIsRecording = false;
    bIsReplaying = false;
    ReplayTime = 0.0f;
    CurrentFrameIndex = 0;
}

void USoccerReplayManager::ApplyFrame(const FReplayFrame& Frame)
{
    UWorld* World = GEngine ? GEngine->GetWorldFromContextObjectChecked(this) : nullptr;
    if (!World)
    {
        return;
    }

    for (const FReplayPlayerState& PlayerState : Frame.PlayerStates)
    {
        ASoccerPlayerCharacter* PlayerActor = FindReplayPlayer(PlayerState);
        if (PlayerActor)
        {
            PlayerActor->SetActorLocationAndRotation(PlayerState.Location, PlayerState.Rotation, false, nullptr, ETeleportType::TeleportPhysics);
            PlayerActor->GetCharacterMovement()->Velocity = PlayerState.Velocity;
        }
    }

    ASoccerBall* ReplayBall = FindReplayBall();
    if (ReplayBall)
    {
        ReplayBall->SetActorLocationAndRotation(Frame.BallState.Location, Frame.BallState.Rotation, false, nullptr, ETeleportType::TeleportPhysics);
        ReplayBall->GetRootComponent()->ComponentVelocity = Frame.BallState.Velocity;
    }
}

ASoccerPlayerCharacter* USoccerReplayManager::FindReplayPlayer(const FReplayPlayerState& PlayerState) const
{
    UWorld* World = GEngine ? GEngine->GetWorldFromContextObjectChecked(this) : nullptr;
    if (!World)
    {
        return nullptr;
    }

    for (TActorIterator<ASoccerPlayerCharacter> It(World); It; ++It)
    {
        ASoccerPlayerCharacter* Player = *It;
        if (Player && Player->GetTeamId() == PlayerState.TeamId && Player->GetPlayerNumber() == PlayerState.PlayerNumber)
        {
            return Player;
        }
    }

    return nullptr;
}

ASoccerBall* USoccerReplayManager::FindReplayBall() const
{
    UWorld* World = GEngine ? GEngine->GetWorldFromContextObjectChecked(this) : nullptr;
    if (!World)
    {
        return nullptr;
    }

    for (TActorIterator<ASoccerBall> It(World); It; ++It)
    {
        return *It;
    }

    return nullptr;
}
