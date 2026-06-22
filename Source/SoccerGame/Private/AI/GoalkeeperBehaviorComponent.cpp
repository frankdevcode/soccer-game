// Copyright Frank Dev Code. All Rights Reserved.

#include "AI/GoalkeeperBehaviorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Ball/SoccerBall.h"
#include "Characters/SoccerPlayerCharacter.h"
#include "Core/SoccerGameSettings.h"

UGoalkeeperBehaviorComponent::UGoalkeeperBehaviorComponent()
    : CheckInterval(0.2f)
    , SaveTriggerDistance(900.0f)
    , OwnerPlayer(nullptr)
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UGoalkeeperBehaviorComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerPlayer = Cast<ASoccerPlayerCharacter>(GetOwner());
    if (!OwnerPlayer)
    {
        return;
    }

    // Only active for goalkeepers
    if (OwnerPlayer->GetPosition() != EPlayerPosition::Goalkeeper)
    {
        return;
    }

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_Check, this, &UGoalkeeperBehaviorComponent::CheckForSave, CheckInterval, true);
    }
}

void UGoalkeeperBehaviorComponent::CheckForSave()
{
    if (!GetWorld() || !OwnerPlayer)
    {
        return;
    }

    TArray<AActor*> Balls;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoccerBall::StaticClass(), Balls);
    if (Balls.Num() == 0)
    {
        return;
    }

    ASoccerBall* Nearest = nullptr;
    float BestDistSq = FLT_MAX;
    const FVector MyLoc = OwnerPlayer->GetActorLocation();

    for (AActor* Actor : Balls)
    {
        const float DistSq = FVector::DistSquared(Actor->GetActorLocation(), MyLoc);
        if (DistSq < BestDistSq)
        {
            BestDistSq = DistSq;
            Nearest = Cast<ASoccerBall>(Actor);
        }
    }

    if (!Nearest)
    {
        return;
    }

    const float Dist = FMath::Sqrt(BestDistSq);
    if (Dist <= SaveTriggerDistance)
    {
        // If ball is in goalkeeper zone, attempt save
        if (OwnerPlayer->IsInGoalkeeperZone())
        {
            OwnerPlayer->DiveForSave();
        }
    }
}
