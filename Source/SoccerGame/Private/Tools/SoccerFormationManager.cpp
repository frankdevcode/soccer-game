// Copyright Frank Dev Code. All Rights Reserved.

#include "Tools/SoccerFormationManager.h"
#include "Game/SoccerGameState.h"
#include "Core/SoccerGameSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/SoccerPlayerCharacter.h"

USoccerFormationManager* USoccerFormationManager::Instance = nullptr;

USoccerFormationManager* USoccerFormationManager::Get()
{
    if (!Instance)
    {
        Instance = NewObject<USoccerFormationManager>();
        Instance->AddToRoot();
    }
    return Instance;
}

void USoccerFormationManager::ApplyDefaultFormation(int32 TeamId)
{
    ApplyFormation(TeamId, TEXT("standard"));
}

void USoccerFormationManager::ApplyFormation(int32 TeamId, const FString& FormationName)
{
    UWorld* World = GEngine ? GEngine->GetWorldFromContextObjectChecked(this) : nullptr;
    if (!World) return;

    ASoccerGameState* GS = World->GetGameState<ASoccerGameState>();
    if (!GS) return;

    TArray<ASoccerPlayerCharacter*> Players = GS->GetRegisteredPlayers();

    // Filter team's players
    TArray<ASoccerPlayerCharacter*> TeamPlayers;
    for (ASoccerPlayerCharacter* P : Players)
    {
        if (P && P->GetTeamId() == TeamId)
        {
            TeamPlayers.Add(P);
        }
    }

    if (TeamPlayers.Num() == 0) return;

    // Get field settings
    USoccerGameSettings* Settings = USoccerGameSettings::Get();
    const float FieldHalfLength = Settings ? Settings->FieldLength * 0.5f : 2000.0f;
    const float FieldHalfWidth = Settings ? Settings->FieldWidth * 0.5f : 1000.0f;
    const float GoalDepth = Settings ? Settings->GoalDepth : 200.0f;

    // Simple formation templates for futsal (5 players): GK + 4 outfield
    TArray<FVector> Offsets;
    if (FormationName.Equals(TEXT("defensive"), ESearchCase::IgnoreCase))
    {
        // More compact: two close defenders, two deep midfield
        Offsets = {
            FVector(0.0f, -FieldHalfLength + GoalDepth + 100.0f, 0.0f), // GK
            FVector(-FieldHalfWidth * 0.3f, -FieldHalfLength * 0.15f, 0.0f), // Defender L
            FVector(FieldHalfWidth * 0.3f, -FieldHalfLength * 0.15f, 0.0f), // Defender R
            FVector(-FieldHalfWidth * 0.15f, FieldHalfLength * 0.15f, 0.0f), // Mid L
            FVector(FieldHalfWidth * 0.15f, FieldHalfLength * 0.15f, 0.0f) // Mid R
        };
    }
    else
    {
        // Standard balanced formation
        Offsets = {
            FVector(0.0f, -FieldHalfLength + GoalDepth + 100.0f, 0.0f), // GK
            FVector(-FieldHalfWidth * 0.4f, -FieldHalfLength * 0.05f, 0.0f), // Def L
            FVector(FieldHalfWidth * 0.4f, -FieldHalfLength * 0.05f, 0.0f), // Def R
            FVector(-FieldHalfWidth * 0.2f, FieldHalfLength * 0.35f, 0.0f), // Att L
            FVector(FieldHalfWidth * 0.2f, FieldHalfLength * 0.35f, 0.0f) // Att R
        };
    }

    // Center position depends on team side: Team 1 at negative Y, Team 2 mirrored
    const bool bTeamIsHome = (TeamId == 1);
    FVector Mirror = bTeamIsHome ? FVector(1.0f, 1.0f, 1.0f) : FVector(1.0f, -1.0f, 1.0f);

    for (int32 i = 0; i < TeamPlayers.Num(); ++i)
    {
        ASoccerPlayerCharacter* P = TeamPlayers[i];
        if (!P) continue;

        int32 idx = FMath::Clamp(i, 0, Offsets.Num()-1);
        FVector TargetOffset = Offsets[idx] * Mirror;

        // Convert to world position relative to pitch center (assume origin 0,0)
        const FVector TargetWorld = TargetOffset;

        P->SetFormationTarget(TargetWorld);
        UE_LOG(LogTemp, Warning, TEXT("[Formation] Team %d: Player %d assigned formation target %s"), TeamId, P->GetPlayerNumber(), *TargetWorld.ToString());
    }
}
