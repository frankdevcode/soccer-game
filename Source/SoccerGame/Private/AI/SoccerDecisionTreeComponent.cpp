// Copyright Frank Dev Code. All Rights Reserved.

#include "AI/SoccerDecisionTreeComponent.h"
#include "Game/SoccerGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Ball/SoccerBall.h"
#include "Characters/SoccerPlayerCharacter.h"

USoccerDecisionTreeComponent::USoccerDecisionTreeComponent()
    : LowStaminaThreshold(25.0f)
    , CloseBallRange(450.0f)
{
    PrimaryComponentTick.bCanEverTick = false;
}

EDecisionState USoccerDecisionTreeComponent::DecideNextState(ASoccerPlayerCharacter* Player)
{
    if (!Player || !GetWorld())
    {
        return EDecisionState::Idle;
    }

    // Stamina-based retreat
    if (Player->GetStamina() <= LowStaminaThreshold)
    {
        return EDecisionState::Retreat;
    }

    // Find nearest ball
    TArray<AActor*> Balls;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoccerBall::StaticClass(), Balls);
    if (Balls.Num() == 0)
    {
        return EDecisionState::Defend;
    }

    AActor* NearestBall = nullptr;
    float BestDistSq = FLT_MAX;
    const FVector MyLoc = Player->GetActorLocation();

    for (AActor* B : Balls)
    {
        const float DistSq = FVector::DistSquared(B->GetActorLocation(), MyLoc);
        if (DistSq < BestDistSq)
        {
            BestDistSq = DistSq;
            NearestBall = B;
        }
    }

    const float Dist = FMath::Sqrt(BestDistSq);

    // Use GameState heuristics for possession
    ASoccerGameState* GS = GetWorld()->GetGameState<ASoccerGameState>();
    const bool bWeHavePossession = GS && GS->GetBallPossession() == Player->GetTeamId();

    // If very close to ball prefer attack/seek
    if (Dist <= CloseBallRange)
    {
        if (bWeHavePossession)
        {
            return EDecisionState::Attack;
        }
        return EDecisionState::SeekBall;
    }

    // If teammate is closer, provide support
    if (GS)
    {
        ASoccerPlayerCharacter* ClosestToBall = nullptr;
        float ClosestDistSq = FLT_MAX;
        for (ASoccerPlayerCharacter* P : GS->GetRegisteredPlayers())
        {
            if (!P) continue;
            const float DistSq = FVector::DistSquared(P->GetActorLocation(), NearestBall->GetActorLocation());
            if (DistSq < ClosestDistSq)
            {
                ClosestDistSq = DistSq;
                ClosestToBall = P;
            }
        }

        if (ClosestToBall && ClosestToBall->GetTeamId() == Player->GetTeamId() && ClosestToBall != Player)
        {
            return EDecisionState::Support;
        }
    }

    // Default defensive posture
    return EDecisionState::Defend;
}
