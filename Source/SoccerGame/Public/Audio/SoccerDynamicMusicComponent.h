// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundBase.h"
#include "SoccerDynamicMusicComponent.generated.h"

class UAudioComponent;

UENUM(BlueprintType)
enum class ESoccerMusicState : uint8
{
	Menu UMETA(DisplayName = "Menu"),
	MatchIntro UMETA(DisplayName = "Match Intro"),
	MatchAction UMETA(DisplayName = "Match Action"),
	MatchPause UMETA(DisplayName = "Match Pause"),
	MatchGoal UMETA(DisplayName = "Match Goal"),
	MatchVictory UMETA(DisplayName = "Match Victory"),
	MatchDefeat UMETA(DisplayName = "Match Defeat")
};

/**
 * @class USoccerDynamicMusicComponent
 * @brief Plays state-driven music for menus, match flow, and results.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SOCCERGAME_API USoccerDynamicMusicComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USoccerDynamicMusicComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	void SetMusicState(ESoccerMusicState NewState);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	void SetEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	void SetMusicVolume(float InVolume);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Audio")
	ESoccerMusicState GetCurrentMusicState() const { return CurrentState; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	bool bEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	float MusicVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	USoundBase* MenuMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	USoundBase* MatchIntroMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	USoundBase* MatchActionMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	USoundBase* MatchPauseMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	USoundBase* MatchGoalMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	USoundBase* MatchVictoryMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Audio")
	USoundBase* MatchDefeatMusic;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Audio")
	UAudioComponent* ActiveMusicComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Audio")
	ESoccerMusicState CurrentState;

private:
	void StopCurrentMusic();
	USoundBase* GetMusicForState(ESoccerMusicState State) const;
};
