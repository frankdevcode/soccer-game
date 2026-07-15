// Copyright Frank Dev Code. All Rights Reserved.

#include "Audio/SoccerDynamicMusicComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

USoccerDynamicMusicComponent::USoccerDynamicMusicComponent()
	: bEnabled(true)
	, MusicVolume(0.6f)
	, MenuMusic(nullptr)
	, MatchIntroMusic(nullptr)
	, MatchActionMusic(nullptr)
	, MatchPauseMusic(nullptr)
	, MatchGoalMusic(nullptr)
	, MatchVictoryMusic(nullptr)
	, MatchDefeatMusic(nullptr)
	, ActiveMusicComponent(nullptr)
	, CurrentState(ESoccerMusicState::Menu)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USoccerDynamicMusicComponent::BeginPlay()
{
	Super::BeginPlay();
	SetMusicState(CurrentState);
}

void USoccerDynamicMusicComponent::SetMusicState(ESoccerMusicState NewState)
{
	if (!bEnabled || NewState == CurrentState)
	{
		return;
	}

	CurrentState = NewState;
	StopCurrentMusic();

	USoundBase* MusicToPlay = GetMusicForState(CurrentState);
	if (!MusicToPlay || !GetWorld())
	{
		return;
	}

	ActiveMusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), MusicToPlay, MusicVolume, 1.0f, 0.0f, nullptr, true, true);
	if (ActiveMusicComponent)
	{
		ActiveMusicComponent->Play();
	}
}

void USoccerDynamicMusicComponent::SetEnabled(bool bEnabledValue)
{
	bEnabled = bEnabledValue;
	if (!bEnabled)
	{
		StopCurrentMusic();
	}
}

void USoccerDynamicMusicComponent::SetMusicVolume(float InVolume)
{
	MusicVolume = FMath::Clamp(InVolume, 0.0f, 1.0f);
	if (ActiveMusicComponent)
	{
		ActiveMusicComponent->SetVolumeMultiplier(MusicVolume);
	}
}

void USoccerDynamicMusicComponent::StopCurrentMusic()
{
	if (ActiveMusicComponent)
	{
		ActiveMusicComponent->Stop();
		ActiveMusicComponent->DestroyComponent();
		ActiveMusicComponent = nullptr;
	}
}

USoundBase* USoccerDynamicMusicComponent::GetMusicForState(ESoccerMusicState State) const
{
	switch (State)
	{
	case ESoccerMusicState::Menu:
		return MenuMusic;
	case ESoccerMusicState::MatchIntro:
		return MatchIntroMusic;
	case ESoccerMusicState::MatchAction:
		return MatchActionMusic;
	case ESoccerMusicState::MatchPause:
		return MatchPauseMusic;
	case ESoccerMusicState::MatchGoal:
		return MatchGoalMusic;
	case ESoccerMusicState::MatchVictory:
		return MatchVictoryMusic;
	case ESoccerMusicState::MatchDefeat:
		return MatchDefeatMusic;
	default:
		return nullptr;
	}
}
