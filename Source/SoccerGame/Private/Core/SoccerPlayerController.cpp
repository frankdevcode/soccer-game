// Copyright Frank Dev Code. All Rights Reserved.

#include "Core/SoccerPlayerController.h"
#include "Characters/SoccerPlayerCharacter.h"
#include "UI/SoccerMainMenuWidget.h"
#include "Blueprint/UserWidget.h"

ASoccerPlayerController::ASoccerPlayerController()
	: bShowHUD(true)
	, bShowPauseMenu(false)
	, MainMenuWidget(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	MainMenuWidgetClass = USoccerMainMenuWidget::StaticClass();
}

void ASoccerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] Player controller initialized"));

	// Setup backend sync timer
	GetWorld()->GetTimerManager().SetTimer(
		BackendSyncTimerHandle,
		this,
		&ASoccerPlayerController::SyncWithBackend,
		1.0f,
		true
	);

	ShowMainMenu();
}

void ASoccerPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoccerPlayerController::PossessPlayerCharacter(ASoccerPlayerCharacter* Character)
{
	ControlledCharacter = Character;
	Possess(Character);

	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] Possessed player character"));
}

void ASoccerPlayerController::ShowMainHUD()
{
	bShowHUD = true;
	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] HUD shown"));
}

void ASoccerPlayerController::HideMainHUD()
{
	bShowHUD = false;
	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] HUD hidden"));
}

void ASoccerPlayerController::ShowPauseMenu()
{
	bShowPauseMenu = true;
	SetPause(true);

	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] Pause menu shown"));
}

void ASoccerPlayerController::HidePauseMenu()
{
	bShowPauseMenu = false;
	SetPause(false);

	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] Pause menu hidden"));
}

void ASoccerPlayerController::ShowMainMenu()
{
	if (MainMenuWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] MainMenuWidgetClass not set"));
		return;
	}

	if (MainMenuWidget == nullptr)
	{
		MainMenuWidget = CreateWidget<USoccerMainMenuWidget>(this, MainMenuWidgetClass);
	}

	if (MainMenuWidget)
	{
		MainMenuWidget->AddToViewport();
		bShowHUD = false;
		SetShowMouseCursor(true);
		SetInputMode(FInputModeUIOnly());
		UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] Main menu shown"));
	}
}

void ASoccerPlayerController::HideMainMenu()
{
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
	}

	bShowHUD = true;
	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());
	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] Main menu hidden"));
}

void ASoccerPlayerController::SendPlayerStateToBackend()
{
	if (!ControlledCharacter)
	{
		return;
	}

	// TODO: Implement backend communication
	// Send current player position, stamina, stats to backend API
	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] Sending player state to backend"));
}

void ASoccerPlayerController::RequestGameStateUpdate()
{
	// TODO: Implement backend communication
	// Request latest game state from backend
	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] Requesting game state update from backend"));
}

void ASoccerPlayerController::SyncWithBackend()
{
	SendPlayerStateToBackend();
}
