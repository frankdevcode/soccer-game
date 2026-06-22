// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoccerFormationManager.generated.h"

class ASoccerPlayerCharacter;

UCLASS()
class SOCCERGAME_API USoccerFormationManager : public UObject
{
    GENERATED_BODY()

public:
    static USoccerFormationManager* Get();

    UFUNCTION(BlueprintCallable, Category = "Soccer|Formation")
    void ApplyDefaultFormation(int32 TeamId);

    UFUNCTION(BlueprintCallable, Category = "Soccer|Formation")
    void ApplyFormation(int32 TeamId, const FString& FormationName);

private:
    static USoccerFormationManager* Instance;
};
