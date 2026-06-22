// Copyright Frank Dev Code. All Rights Reserved.

#include "Gameplay/SpecialMoveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Ball/SoccerBall.h"
#include "GameFramework/Actor.h"

USpecialMoveComponent::USpecialMoveComponent()
    : CooldownSeconds(5.0f)
    , LastExecutedTime(-10000.0f)
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USpecialMoveComponent::BeginPlay()
{
    Super::BeginPlay();
}

bool USpecialMoveComponent::CanExecuteMove() const
{
    const UWorld* World = GetWorld();
    const float CurrentTime = World ? World->GetTimeSeconds() : 0.0f;
    return (CurrentTime - LastExecutedTime) >= CooldownSeconds;
}

void USpecialMoveComponent::TriggerMove(FName MoveName)
{
    if (!CanExecuteMove())
    {
        return;
    }

    LastExecutedTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
    ExecuteMove(MoveName);
}

void USpecialMoveComponent::ExecuteMove(FName MoveName)
{
    // Example default special move: an empowered chip/volley applied to nearest ball
    AActor* Owner = GetOwner();
    if (!Owner || !GetWorld())
    {
        return;
    }

    TArray<AActor*> FoundBalls;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoccerBall::StaticClass(), FoundBalls);

    AActor* NearestBall = nullptr;
    float BestDist = FLT_MAX;
    const FVector OwnerLoc = Owner->GetActorLocation();

    for (AActor* Actor : FoundBalls)
    {
        const float Dist = FVector::DistSquared(Actor->GetActorLocation(), OwnerLoc);
        if (Dist < BestDist)
        {
            BestDist = Dist;
            NearestBall = Actor;
        }
    }

    if (!NearestBall)
    {
        return;
    }

    ASoccerBall* Ball = Cast<ASoccerBall>(NearestBall);
    if (!Ball)
    {
        return;
    }

    // Apply a flashy upward-chipped kick for the special move
    const FVector Dir = (Ball->GetActorLocation() - OwnerLoc).GetSafeNormal();
    const FVector UpChip = (Dir + FVector::UpVector * 0.8f).GetSafeNormal();
    Ball->ApplyKick(UpChip, 1.4f);
    UE_LOG(LogTemp, Warning, TEXT("[SpecialMoveComponent] Executed special move '%s'"), *MoveName.ToString());

    // Notify owner animation instance (if any)
    AActor* Owner = GetOwner();
    if (Owner)
    {
        USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
        if (Mesh)
        {
            if (UAnimInstance* Anim = Mesh->GetAnimInstance())
            {
                if (USoccerPlayerAnimInstance* SoccerAnim = Cast<USoccerPlayerAnimInstance>(Anim))
                {
                    SoccerAnim->PlaySpecialMove(MoveName);
                }
            }
        }
    }
}
