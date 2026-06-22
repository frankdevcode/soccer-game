// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SimpleAIController.generated.h"

class ASoccerBall;
class ASoccerPlayerCharacter;

UCLASS()
class SOCCERGAME_API ASimpleAIController : public AAIController
{
    GENERATED_BODY()

public:
    ASimpleAIController();

    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;

protected:
    virtual void BeginPlay() override;

private:
    void SearchForBall();
    FTimerHandle SearchTimerHandle;
    float SearchInterval;

    ASoccerPlayerCharacter* ControlledPlayer;
};
