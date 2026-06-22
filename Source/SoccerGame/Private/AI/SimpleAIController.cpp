// Copyright Frank Dev Code. All Rights Reserved.

#include "AI/SimpleAIController.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Ball/SoccerBall.h"
#include "Characters/SoccerPlayerCharacter.h"

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
        GetWorld()->GetTimerManager().SetTimer(SearchTimerHandle, this, &ASimpleAIController::SearchForBall, SearchInterval, true);
    }
}

void ASimpleAIController::OnUnPossess()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(SearchTimerHandle);
    }

    ControlledPlayer = nullptr;
    Super::OnUnPossess();
}

void ASimpleAIController::SearchForBall()
{
    if (!GetPawn() || !ControlledPlayer)
    {
        return;
    }

    TArray<AActor*> FoundBalls;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoccerBall::StaticClass(), FoundBalls);

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
