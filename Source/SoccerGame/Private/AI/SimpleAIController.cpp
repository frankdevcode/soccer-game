// Copyright Frank Dev Code. All Rights Reserved.

#include "AI/SimpleAIController.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Ball/SoccerBall.h"
#include "Characters/SoccerPlayerCharacter.h"
#include "AI/TacticalDecisionComponent.h"
#include "AI/SoccerDecisionTreeComponent.h"

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
    if (!ControlledPlayer || !GetWorld())
    {
        return;
    }

    // Find nearest ball in the world
    TArray<AActor*> Balls;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoccerBall::StaticClass(), Balls);
    if (Balls.Num() == 0)
    {
        return;
    }

    AActor* NearestBall = nullptr;
    float BestDistSq = FLT_MAX;
    const FVector MyLoc = ControlledPlayer->GetActorLocation();

    for (AActor* Actor : Balls)
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

    // Use decision tree if available
    if (USoccerDecisionTreeComponent* DT = ControlledPlayer->GetDecisionTreeComponent())
    {
        const EDecisionState State = DT->DecideNextState(ControlledPlayer);
        switch (State)
        {
        case EDecisionState::Attack:
            if (BestDistSq <= FMath::Square(ControlledPlayer->GetTacticalDecisionComponent() ? ControlledPlayer->GetTacticalDecisionComponent()->AttackRange : 400.0f))
            {
                ControlledPlayer->KickBall(0.95f);
            }
            else
            {
                MoveToActor(NearestBall, 50.0f, true, true, true, 0, true);
            }
            break;
        case EDecisionState::SeekBall:
            MoveToActor(NearestBall, 50.0f, true, true, true, 0, true);
            break;
        case EDecisionState::Support:
            {
                const FVector SupportPos = NearestBall->GetActorLocation() + FVector(200.0f, 0.0f, 0.0f);
                MoveToLocation(SupportPos, 50.0f, true, true, true, false, 0, true);
            }
            break;
        case EDecisionState::Retreat:
            MoveToLocation(ControlledPlayer->GetFormationTarget(), 50.0f, true, true, true, false, 0, true);
            break;
        case EDecisionState::Defend:
        default:
            StopMovement();
            break;
        }
        return;
    }

    // Fallback: tactical component or simple approach
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
                const FVector SupportPos = NearestBall->GetActorLocation() + FVector(200.0f, 0.0f, 0.0f);
                MoveToLocation(SupportPos, 50.0f, true, true, true, false, 0, true);
            }
            break;
        case ETacticalAction::Defend:
        default:
            StopMovement();
            break;
        }
        return;
    }

    // Simple fallback: go to ball
    const float CloseRangeSq = FMath::Square(150.0f);
    if (BestDistSq <= CloseRangeSq)
    {
        ControlledPlayer->KickBall(0.9f);
    }
    else
    {
        MoveToActor(NearestBall, 50.0f, true, true, true, 0, true);
    }
}
