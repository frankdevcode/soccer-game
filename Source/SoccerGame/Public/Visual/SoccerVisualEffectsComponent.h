// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SoccerVisualEffectsComponent.generated.h"

UENUM(BlueprintType)
enum class ESoccerVisualPreset : uint8
{
	Default UMETA(DisplayName = "Default"),
	MatchDay UMETA(DisplayName = "Match Day"),
	Evening UMETA(DisplayName = "Evening"),
	Night UMETA(DisplayName = "Night")
};

/**
 * @class USoccerVisualEffectsComponent
 * @brief Central component for lighting, particles, post-processing, weather, and polish.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SOCCERGAME_API USoccerVisualEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USoccerVisualEffectsComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Soccer|Visual")
	void ApplyPreset(ESoccerVisualPreset Preset);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Visual")
	void SetBrightness(float Value);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Visual")
	void SetContrast(float Value);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Visual")
	void SetWeatherIntensity(float Value);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Visual")
	void EnableParticles(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Visual")
	void EnableWeather(bool bEnabled);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Visual")
	float Brightness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Visual")
	float Contrast;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Visual")
	float WeatherIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Visual")
	bool bParticlesEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Visual")
	bool bWeatherEnabled;

private:
	void ApplyLightingPreset(ESoccerVisualPreset Preset);
	void ApplyPostProcessPreset(ESoccerVisualPreset Preset);
	void ApplyWeatherPreset(ESoccerVisualPreset Preset);
	void ApplyParticlePreset(ESoccerVisualPreset Preset);
};
