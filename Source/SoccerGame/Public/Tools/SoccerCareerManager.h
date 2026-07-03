// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoccerCareerManager.generated.h"

USTRUCT(BlueprintType)
struct FSoccerPlayerProfile
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Career")
    FString PlayerId;

    UPROPERTY(BlueprintReadWrite, Category = "Career")
    FString PlayerName;

    UPROPERTY(BlueprintReadWrite, Category = "Career")
    int32 Level;

    UPROPERTY(BlueprintReadWrite, Category = "Career")
    int32 Experience;

    UPROPERTY(BlueprintReadWrite, Category = "Career")
    int32 CurrentTeamId;

    FSoccerPlayerProfile()
        : Level(1), Experience(0), CurrentTeamId(0)
    {}
};

UCLASS()
class SOCCERGAME_API USoccerCareerManager : public UObject
{
    GENERATED_BODY()

public:
    static USoccerCareerManager* Get();

    UFUNCTION(BlueprintCallable, Category = "Career")
    void StartCareer(const FString& PlayerName);

    UFUNCTION(BlueprintCallable, Category = "Career")
    FSoccerPlayerProfile CreatePlayerProfile(const FString& PlayerName);

    UFUNCTION(BlueprintCallable, Category = "Career")
    bool SaveProfile(const FSoccerPlayerProfile& Profile);

    UFUNCTION(BlueprintCallable, Category = "Career")
    bool LoadProfile(const FString& PlayerId, FSoccerPlayerProfile& OutProfile) const;

    UFUNCTION(BlueprintCallable, Category = "Career")
    void AdvanceSeason();

private:
    static USoccerCareerManager* Instance;

    TMap<FString, FSoccerPlayerProfile> Profiles;
};
