// Copyright Frank Dev Code. All Rights Reserved.

#include "Visual/SoccerVisualEffectsComponent.h"

USoccerVisualEffectsComponent::USoccerVisualEffectsComponent()
	: Brightness(1.0f)
	, Contrast(1.0f)
	, WeatherIntensity(0.0f)
	, bParticlesEnabled(true)
	, bWeatherEnabled(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USoccerVisualEffectsComponent::BeginPlay()
{
	Super::BeginPlay();
	ApplyPreset(ESoccerVisualPreset::Default);
}

void USoccerVisualEffectsComponent::ApplyPreset(ESoccerVisualPreset Preset)
{
	ApplyLightingPreset(Preset);
	ApplyPostProcessPreset(Preset);
	ApplyWeatherPreset(Preset);
	ApplyParticlePreset(Preset);
}

void USoccerVisualEffectsComponent::SetBrightness(float Value)
{
	Brightness = FMath::Clamp(Value, 0.3f, 2.0f);
}

void USoccerVisualEffectsComponent::SetContrast(float Value)
{
	Contrast = FMath::Clamp(Value, 0.5f, 2.0f);
}

void USoccerVisualEffectsComponent::SetWeatherIntensity(float Value)
{
	WeatherIntensity = FMath::Clamp(Value, 0.0f, 1.0f);
	bWeatherEnabled = WeatherIntensity > 0.0f;
}

void USoccerVisualEffectsComponent::EnableParticles(bool bEnabled)
{
	bParticlesEnabled = bEnabled;
}

void USoccerVisualEffectsComponent::EnableWeather(bool bEnabled)
{
	bWeatherEnabled = bEnabled;
	if (!bEnabled)
	{
		WeatherIntensity = 0.0f;
	}
}

void USoccerVisualEffectsComponent::ApplyLightingPreset(ESoccerVisualPreset Preset)
{
	switch (Preset)
	{
	case ESoccerVisualPreset::MatchDay:
		Brightness = 1.15f;
		Contrast = 1.05f;
		break;
	case ESoccerVisualPreset::Evening:
		Brightness = 0.95f;
		Contrast = 1.1f;
		break;
	case ESoccerVisualPreset::Night:
		Brightness = 0.75f;
		Contrast = 1.2f;
		break;
	default:
		Brightness = 1.0f;
		Contrast = 1.0f;
		break;
	}
}

void USoccerVisualEffectsComponent::ApplyPostProcessPreset(ESoccerVisualPreset Preset)
{
	// Placeholder for future UE post-process integration.
	// The component exposes the values so they can be bound to BP/scene materials later.
	UE_LOG(LogTemp, Warning, TEXT("[SoccerVisualEffectsComponent] Post-process preset applied: %d"), static_cast<int32>(Preset));
}

void USoccerVisualEffectsComponent::ApplyWeatherPreset(ESoccerVisualPreset Preset)
{
	switch (Preset)
	{
	case ESoccerVisualPreset::Evening:
		WeatherIntensity = 0.25f;
		bWeatherEnabled = true;
		break;
	case ESoccerVisualPreset::Night:
		WeatherIntensity = 0.35f;
		bWeatherEnabled = true;
		break;
	default:
		WeatherIntensity = 0.0f;
		bWeatherEnabled = false;
		break;
	}
}

void USoccerVisualEffectsComponent::ApplyParticlePreset(ESoccerVisualPreset Preset)
{
	switch (Preset)
	{
	case ESoccerVisualPreset::MatchDay:
		bParticlesEnabled = true;
		break;
	case ESoccerVisualPreset::Evening:
		bParticlesEnabled = true;
		break;
	case ESoccerVisualPreset::Night:
		bParticlesEnabled = true;
		break;
	default:
		bParticlesEnabled = true;
		break;
	}
}
