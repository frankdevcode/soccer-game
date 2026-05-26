// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SoccerPlayerController.generated.h"

class ASoccerPlayerCharacter;

/**
 * @class ASoccerPlayerController
 * @brief Player controller for Soccer-Game
 * 
 * Handles input, camera, and player-specific logic.
 * Communicates with backend API for player state synchronization.
 */
UCLASS()
class SOCCERGAME_API ASoccerPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASoccerPlayerController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Player control
	UFUNCTION(BlueprintCallable, Category = "Soccer|Controller")
	void PossessPlayerCharacter(ASoccerPlayerCharacter* Character);

	// HUD and UI
	UFUNCTION(BlueprintCallable, Category = "Soccer|UI")
	void ShowMainHUD();

	UFUNCTION(BlueprintCallable, Category = "Soccer|UI")
	void HideMainHUD();

	UFUNCTION(BlueprintCallable, Category = "Soccer|UI")
	void ShowPauseMenu();

	UFUNCTION(BlueprintCallable, Category = "Soccer|UI")
	void HidePauseMenu();

	// Backend communication
	UFUNCTION(BlueprintCallable, Category = "Soccer|Backend")
	void SendPlayerStateToBackend();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Backend")
	void RequestGameStateUpdate();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Controller")
	class ASoccerPlayerCharacter* ControlledCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|UI")
	bool bShowHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|UI")
	bool bShowPauseMenu;

private:
	// Synchronization timer
	FTimerHandle BackendSyncTimerHandle;

	void SyncWithBackend();
};
