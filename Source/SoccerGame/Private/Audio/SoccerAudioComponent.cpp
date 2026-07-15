// Copyright Frank Dev Code. All Rights Reserved.

#include "Audio/SoccerAudioComponent.h"
#include "Components/AudioComponent.h"
#include "Core/SoccerGameSettings.h"
#include "Kismet/GameplayStatics.h"

USoccerAudioComponent::USoccerAudioComponent()
	: bAudioEnabled(true)
	, MasterVolume(1.0f)
	, SFXVolume(1.0f)
	, AmbientVolume(0.8f)
	, KickSound(nullptr)
	, PassSound(nullptr)
	, HeaderSound(nullptr)
	, GoalSound(nullptr)
	, CrowdAmbientSound(nullptr)
	, FootstepSound(nullptr)
	, CollisionSound(nullptr)
	, SaveSound(nullptr)
	, AmbientLoopComponent(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.0f;
}

void USoccerAudioComponent::BeginPlay()
{
	Super::BeginPlay();
	ApplySettingsFromGame();
}

void USoccerAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USoccerAudioComponent::SetAudioEnabled(bool bEnabled)
{
	bAudioEnabled = bEnabled;
	if (!bAudioEnabled && AmbientLoopComponent)
	{
		AmbientLoopComponent->Stop();
	}
}

void USoccerAudioComponent::SetVolumeSettings(float InMasterVolume, float InSFXVolume, float InAmbientVolume)
{
	MasterVolume = FMath::Clamp(InMasterVolume, 0.0f, 1.0f);
	SFXVolume = FMath::Clamp(InSFXVolume, 0.0f, 1.0f);
	AmbientVolume = FMath::Clamp(InAmbientVolume, 0.0f, 1.0f);

	if (AmbientLoopComponent)
	{
		AmbientLoopComponent->SetVolumeMultiplier(AmbientVolume * MasterVolume);
	}
}

UAudioComponent* USoccerAudioComponent::PlayCue(USoundBase* Sound, const FVector& Location, float VolumeMultiplier, float PitchMultiplier)
{
	if (!bAudioEnabled || !Sound || !GetOwner())
	{
		return nullptr;
	}

	UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(
		GetWorld(),
		Sound,
		Location.IsNearlyZero() ? GetOwner()->GetActorLocation() : Location,
		FRotator::ZeroRotator,
		GetEffectiveVolume(VolumeMultiplier),
		PitchMultiplier,
		0.0f,
		nullptr,
		nullptr,
		true);

	return AudioComponent;
}

void USoccerAudioComponent::PlayKickSound()
{
	PlayCue(KickSound);
}

void USoccerAudioComponent::PlayPassSound()
{
	PlayCue(PassSound);
}

void USoccerAudioComponent::PlayHeaderSound()
{
	PlayCue(HeaderSound);
}

void USoccerAudioComponent::PlayGoalSound()
{
	PlayCue(GoalSound);
}

void USoccerAudioComponent::PlayCrowdAmbientLoop()
{
	if (!bAudioEnabled || !CrowdAmbientSound || AmbientLoopComponent)
	{
		return;
	}

	AmbientLoopComponent = UGameplayStatics::SpawnSound2D(GetWorld(), CrowdAmbientSound, AmbientVolume * MasterVolume, 1.0f, 0.0f, nullptr, true, true);
	AmbientLoopComponent->Play();
}

void USoccerAudioComponent::StopCrowdAmbientLoop()
{
	if (AmbientLoopComponent)
	{
		AmbientLoopComponent->Stop();
		AmbientLoopComponent = nullptr;
	}
}

void USoccerAudioComponent::PlayFootstepSound()
{
	PlayCue(FootstepSound);
}

void USoccerAudioComponent::PlayCollisionSound()
{
	PlayCue(CollisionSound);
}

void USoccerAudioComponent::PlaySaveSound()
{
	PlayCue(SaveSound);
}

void USoccerAudioComponent::ApplySettingsFromGame()
{
	if (USoccerGameSettings* Settings = USoccerGameSettings::Get())
	{
		SetVolumeSettings(1.0f, 1.0f, 0.75f);
	}
}

float USoccerAudioComponent::GetEffectiveVolume(float VolumeMultiplier) const
{
	return FMath::Clamp(MasterVolume * SFXVolume * VolumeMultiplier, 0.0f, 1.0f);
}
