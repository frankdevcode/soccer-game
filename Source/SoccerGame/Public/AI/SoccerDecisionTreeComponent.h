// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SoccerDecisionTreeComponent.generated.h"

class ASoccerPlayerCharacter;

UENUM(BlueprintType)
enum class EDecisionState : uint8
{
    Idle UMETA(DisplayName = "Idle"),
    SeekBall UMETA(DisplayName = "SeekBall"),
    Attack UMETA(DisplayName = "Attack"),
    Support UMETA(DisplayName = "Support"),
    Defend UMETA(DisplayName = "Defend"),
    Retreat UMETA(DisplayName = "Retreat")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOCCERGAME_API USoccerDecisionTreeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USoccerDecisionTreeComponent();

    UFUNCTION(BlueprintCallable, Category = "Soccer|AI")
    EDecisionState DecideNextState(ASoccerPlayerCharacter* Player);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|AI")
    float LowStaminaThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|AI")
    float CloseBallRange;
};
