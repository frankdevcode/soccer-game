// Copyright Frank Dev Code. All Rights Reserved.

#include "Tools/SoccerCareerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Guid.h"

USoccerCareerManager* USoccerCareerManager::Instance = nullptr;

USoccerCareerManager* USoccerCareerManager::Get()
{
    if (!Instance)
    {
        Instance = NewObject<USoccerCareerManager>();
        Instance->AddToRoot();
    }
    return Instance;
}

void USoccerCareerManager::StartCareer(const FString& PlayerName)
{
    FSoccerPlayerProfile Profile = CreatePlayerProfile(PlayerName);
    SaveProfile(Profile);

    UE_LOG(LogTemp, Warning, TEXT("[Career] Career started for %s (ID: %s)"), *Profile.PlayerName, *Profile.PlayerId);

    // TODO: Load career UI, initialize progression systems, and persist data
}

FSoccerPlayerProfile USoccerCareerManager::CreatePlayerProfile(const FString& PlayerName)
{
    FSoccerPlayerProfile Profile;
    Profile.PlayerId = FGuid::NewGuid().ToString();
    Profile.PlayerName = PlayerName;
    Profile.Level = 1;
    Profile.Experience = 0;
    Profile.CurrentTeamId = 0; // unsigned/unassigned

    Profiles.Add(Profile.PlayerId, Profile);
    return Profile;
}

bool USoccerCareerManager::SaveProfile(const FSoccerPlayerProfile& Profile)
{
    Profiles.Add(Profile.PlayerId, Profile);
    // In future: persist to disk or backend
    UE_LOG(LogTemp, Warning, TEXT("[Career] Saved profile %s"), *Profile.PlayerId);
    return true;
}

bool USoccerCareerManager::LoadProfile(const FString& PlayerId, FSoccerPlayerProfile& OutProfile) const
{
    if (const FSoccerPlayerProfile* Found = Profiles.Find(PlayerId))
    {
        OutProfile = *Found;
        return true;
    }
    return false;
}

void USoccerCareerManager::AdvanceSeason()
{
    // Placeholder: update contracts, simulate transfers, progress seasons
    UE_LOG(LogTemp, Warning, TEXT("[Career] Season advanced (placeholder)."));
}
