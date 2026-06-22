// Copyright Frank Dev Code. All Rights Reserved.

#include "AI/SimpleAIController.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Ball/SoccerBall.h"
#include "Characters/SoccerPlayerCharacter.h"
#include "AI/TacticalDecisionComponent.h"

ASimpleAIController::ASimpleAIController()
    : SearchInterval(0.5f)
    , ControlledPlayer(nullptr)
{
    PrimaryActorTick.bCanEverTick = false;
}

void ASimpleAIController::BeginPlay()
{
    Super::BeginPlay();
}

void ASimpleAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    ControlledPlayer = Cast<ASoccerPlayerCharacter>(InPawn);

    if (GetWorld())
    {
        return;
    }
    AActor* NearestBall = nullptr;
    float BestDistSq = FLT_MAX;
    const FVector MyLoc = GetPawn()->GetActorLocation();

    for (AActor* Actor : FoundBalls)
    {
        const float DistSq = FVector::DistSquared(Actor->GetActorLocation(), MyLoc);
        if (DistSq < BestDistSq)
        {
            BestDistSq = DistSq;
            NearestBall = Actor;
        }
    }

    if (!NearestBall)
    {
        return;
    }

    // Tactical decision
    if (ControlledPlayer)
    {
        UTacticalDecisionComponent* Tactic = ControlledPlayer->GetTacticalDecisionComponent();
        if (Tactic)
        {
            const ETacticalAction Action = Tactic->DecideNextAction();
            const float CloseRangeSq = FMath::Square(150.0f);

            switch (Action)
            {
            case ETacticalAction::Attack:
                if (BestDistSq <= FMath::Square(Tactic->AttackRange))
                {
                    ControlledPlayer->KickBall(0.95f);
                }
                else
                {
                    MoveToActor(NearestBall, 50.0f, true, true, true, 0, true);
                }
                break;
            case ETacticalAction::SeekBall:
            case ETacticalAction::Press:
                MoveToActor(NearestBall, 50.0f, true, true, true, 0, true);
                break;
            case ETacticalAction::Support:
                {
                    // Support: move to offset position relative to the nearest teammate or ball
                    const FVector SupportPos = NearestBall->GetActorLocation() + FVector(200.0f, 0.0f, 0.0f);
                    MoveToLocation(SupportPos, 50.0f, true, true, true, false, 0, true);
                }
                break;
            case ETacticalAction::Defend:
            default:
                // Fallback: return to default or hold position
                StopMovement();
                break;
            }
            return;
        }
    }

    const float CloseRangeSq = FMath::Square(150.0f);
    if (BestDistSq <= CloseRangeSq)
    {
        ControlledPlayer->KickBall(0.9f);
    }
    else
    {
        MoveToActor(NearestBall, 50.0f, true, true, true, 0, true);
    }
        const float DistSq = FVector::DistSquared(Actor->GetActorLocation(), MyLoc);
        if (DistSq < BestDistSq)
        {
            BestDistSq = DistSq;
            NearestBall = Actor;
        }
    }

    if (!NearestBall)
    {
        return;
    }

    const float CloseRangeSq = FMath::Square(150.0f);
    if (BestDistSq <= CloseRangeSq)
    {
        // Close enough: attempt a kick
        ControlledPlayer->KickBall(0.9f);
    }
    else
    {
        // Move towards the ball
        MoveToActor(NearestBall, 50.0f, true, true, true, 0, true);
    }
}
