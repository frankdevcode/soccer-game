// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundBase.h"
#include "SoccerAudioComponent.generated.h"

class UAudioComponent;

/**
 * @class USoccerAudioComponent
 * @brief Reusable 3D audio component for soccer gameplay events.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SOCCERGAME_API USoccerAudioComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USoccerAudioComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	void SetAudioEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	bool IsAudioEnabled() const { return bAudioEnabled; }

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	void SetVolumeSettings(float InMasterVolume, float InSFXVolume, float InAmbientVolume);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	UAudioComponent* PlayCue(USoundBase* Sound, const FVector& Location = FVector::ZeroVector, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	void PlayKickSound();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	void PlayPassSound();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	void PlayHeaderSound();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	void PlayGoalSound();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	void PlayCrowdAmbientLoop();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	void StopCrowdAmbientLoop();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	void PlayFootstepSound();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	void PlayCollisionSound();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	void PlaySaveSound();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	bool bAudioEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	float MasterVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	float SFXVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	float AmbientVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	USoundBase* KickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	USoundBase* PassSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	USoundBase* HeaderSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	USoundBase* GoalSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	USoundBase* CrowdAmbientSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	USoundBase* FootstepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	USoundBase* CollisionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	USoundBase* SaveSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Audio")
	UAudioComponent* AmbientLoopComponent;

private:
	void ApplySettingsFromGame();
	float GetEffectiveVolume(float VolumeMultiplier) const;
};
