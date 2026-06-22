// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoalkeeperBehaviorComponent.generated.h"

class ASoccerBall;
class ASoccerPlayerCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOCCERGAME_API UGoalkeeperBehaviorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGoalkeeperBehaviorComponent();

    virtual void BeginPlay() override;

protected:
    void CheckForSave();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Goalkeeper")
    float CheckInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Goalkeeper")
    float SaveTriggerDistance;

private:
    FTimerHandle TimerHandle_Check;
    ASoccerPlayerCharacter* OwnerPlayer;
};
