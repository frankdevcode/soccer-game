// Copyright Frank Dev Code. All Rights Reserved.

#include "AI/TacticalDecisionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Ball/SoccerBall.h"
#include "Game/SoccerGameState.h"
#include "Characters/SoccerPlayerCharacter.h"

UTacticalDecisionComponent::UTacticalDecisionComponent()
    : SupportDistance(600.0f)
    , AttackRange(500.0f)
{
    PrimaryComponentTick.bCanEverTick = false;
}

ETacticalAction UTacticalDecisionComponent::DecideNextAction()
{
    AActor* Owner = GetOwner();
    if (!Owner || !GetWorld())
    {
        return ETacticalAction::Defend;
    }

    ASoccerPlayerCharacter* Player = Cast<ASoccerPlayerCharacter>(Owner);
    if (!Player)
    {
        return ETacticalAction::Defend;
    }

    // Find nearest ball
    TArray<AActor*> Balls;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoccerBall::StaticClass(), Balls);
    if (Balls.Num() == 0)
    {
        return ETacticalAction::Defend;
    }

    AActor* NearestBall = nullptr;
    float BestDistSq = FLT_MAX;
    const FVector MyLoc = Owner->GetActorLocation();

    for (AActor* B : Balls)
    {
        const float DistSq = FVector::DistSquared(B->GetActorLocation(), MyLoc);
        if (DistSq < BestDistSq)
        {
            BestDistSq = DistSq;
            NearestBall = B;
        }
    }

    if (!NearestBall)
    {
        return ETacticalAction::Defend;
    }

    // Access game state to find nearest teammate/opponent to the ball
    ASoccerGameState* GS = GetWorld()->GetGameState<ASoccerGameState>();
    if (!GS)
    {
        return ETacticalAction::SeekBall;
    }

    ASoccerPlayerCharacter* ClosestToBallPlayer = nullptr;
    float ClosestDistPlayerSq = FLT_MAX;
    for (ASoccerPlayerCharacter* P : GS->GetRegisteredPlayers())
    {
        if (!P) continue;
        const float DistSq = FVector::DistSquared(P->GetActorLocation(), NearestBall->GetActorLocation());
        if (DistSq < ClosestDistPlayerSq)
        {
            ClosestDistPlayerSq = DistSq;
            ClosestToBallPlayer = P;
        }
    }

    // Decision heuristics
    const float MyDist = FMath::Sqrt(BestDistSq);
    const float ClosestDist = ClosestToBallPlayer ? FMath::Sqrt(ClosestDistPlayerSq) : FLT_MAX;

    // If I'm closest, attack or take the ball
    if (MyDist <= ClosestDist)
    {
        if (MyDist <= AttackRange)
        {
            return ETacticalAction::Attack;
        }
        return ETacticalAction::SeekBall;
    }

    // If teammate is closer, support (move into open space)
    if (ClosestToBallPlayer && ClosestToBallPlayer->GetTeamId() == Player->GetTeamId())
    {
        return ETacticalAction::Support;
    }

    // Otherwise press the opponent who has the ball
    return ETacticalAction::Press;
}
