// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "SoccerPlayerController.generated.h"

class ASoccerPlayerCharacter;
class USoccerMainMenuWidget;
class USoccerInGameHUDWidget;
class USoccerPlayerCreationWidget;
class USoccerSettingsWidget;
class USoccerStatisticsWidget;

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
	void ShowInGameHUD();

	UFUNCTION(BlueprintCallable, Category = "Soccer|UI")
	void HideInGameHUD();

	UFUNCTION(BlueprintCallable, Category = "Soccer|UI")
	void ShowPauseMenu();

	UFUNCTION(BlueprintCallable, Category = "Soccer|UI")
	void HidePauseMenu();

	UFUNCTION(BlueprintCallable, Category = "Soccer|UI")
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable, Category = "Soccer|UI")
	void HideMainMenu();

	UFUNCTION(BlueprintCallable, Category = "Soccer|UI")
	void ShowPlayerCreationScreen();

	UFUNCTION(BlueprintCallable, Category = "Soccer|UI")
	void HidePlayerCreationScreen();

	UFUNCTION(BlueprintCallable, Category = "Soccer|UI")
	void ShowSettingsScreen();

	UFUNCTION(BlueprintCallable, Category = "Soccer|UI")
	void HideSettingsScreen();

	UFUNCTION(BlueprintCallable, Category = "Soccer|UI")
	void ShowStatisticsScreen();

	UFUNCTION(BlueprintCallable, Category = "Soccer|UI")
	void HideStatisticsScreen();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> InGameHUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> PlayerCreationWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> SettingsWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> StatisticsWidgetClass;

	UPROPERTY()
	USoccerMainMenuWidget* MainMenuWidget;

	UPROPERTY()
	USoccerInGameHUDWidget* InGameHUDWidget;

	UPROPERTY()
	USoccerPlayerCreationWidget* PlayerCreationWidget;

	UPROPERTY()
	USoccerSettingsWidget* SettingsWidget;

	UPROPERTY()
	USoccerStatisticsWidget* StatisticsWidget;

private:
	// Synchronization timer
	FTimerHandle BackendSyncTimerHandle;

	void SyncWithBackend();
};
