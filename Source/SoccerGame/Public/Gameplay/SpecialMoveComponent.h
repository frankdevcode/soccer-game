// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpecialMoveComponent.generated.h"

class ASoccerBall;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOCCERGAME_API USpecialMoveComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USpecialMoveComponent();

    UFUNCTION(BlueprintCallable, Category = "Soccer|SpecialMove")
    bool CanExecuteMove() const;

    UFUNCTION(BlueprintCallable, Category = "Soccer|SpecialMove")
    void TriggerMove(FName MoveName);

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void ExecuteMove(FName MoveName);

    // Cooldown in seconds between special moves
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|SpecialMove")
    float CooldownSeconds;

private:
    float LastExecutedTime;
};
