// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TacticalDecisionComponent.generated.h"

UENUM(BlueprintType)
enum class ETacticalAction : uint8
{
    SeekBall UMETA(DisplayName = "SeekBall"),
    Support UMETA(DisplayName = "Support"),
    Press UMETA(DisplayName = "Press"),
    Defend UMETA(DisplayName = "Defend"),
    Attack UMETA(DisplayName = "Attack")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOCCERGAME_API UTacticalDecisionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTacticalDecisionComponent();

    UFUNCTION(BlueprintCallable, Category = "Soccer|AI")
    ETacticalAction DecideNextAction();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|AI")
    float SupportDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|AI")
    float AttackRange;
};
