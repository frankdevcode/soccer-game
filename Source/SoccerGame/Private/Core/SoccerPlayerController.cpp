// Copyright Frank Dev Code. All Rights Reserved.

#include "Core/SoccerPlayerController.h"
#include "Characters/SoccerPlayerCharacter.h"
#include "UI/SoccerMainMenuWidget.h"
#include "UI/SoccerInGameHUDWidget.h"
#include "UI/SoccerPlayerCreationWidget.h"
#include "UI/SoccerSettingsWidget.h"
#include "UI/SoccerStatisticsWidget.h"
#include "Audio/SoccerDynamicMusicComponent.h"
#include "Visual/SoccerVisualEffectsComponent.h"
#include "Blueprint/UserWidget.h"

ASoccerPlayerController::ASoccerPlayerController()
	: bShowHUD(true)
	, bShowPauseMenu(false)
	, MainMenuWidget(nullptr)
	, InGameHUDWidget(nullptr)
	, PlayerCreationWidget(nullptr)
	, SettingsWidget(nullptr)
	, StatisticsWidget(nullptr)
	, DynamicMusicComponent(nullptr)
	, VisualEffectsComponent(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	MainMenuWidgetClass = USoccerMainMenuWidget::StaticClass();
	InGameHUDWidgetClass = USoccerInGameHUDWidget::StaticClass();
	PlayerCreationWidgetClass = USoccerPlayerCreationWidget::StaticClass();
	SettingsWidgetClass = USoccerSettingsWidget::StaticClass();
	StatisticsWidgetClass = USoccerStatisticsWidget::StaticClass();
	DynamicMusicComponent = CreateDefaultSubobject<USoccerDynamicMusicComponent>(TEXT("DynamicMusicComponent"));
	VisualEffectsComponent = CreateDefaultSubobject<USoccerVisualEffectsComponent>(TEXT("VisualEffectsComponent"));
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

	if (DynamicMusicComponent)
	{
		DynamicMusicComponent->SetMusicState(ESoccerMusicState::Menu);
	}

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

void ASoccerPlayerController::ShowInGameHUD()
{
	if (InGameHUDWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] InGameHUDWidgetClass not set"));
		return;
	}

	if (InGameHUDWidget == nullptr)
	{
		InGameHUDWidget = CreateWidget<USoccerInGameHUDWidget>(this, InGameHUDWidgetClass);
	}

	if (InGameHUDWidget)
	{
		InGameHUDWidget->AddToViewport();
		bShowHUD = true;
		if (DynamicMusicComponent)
		{
			DynamicMusicComponent->SetMusicState(ESoccerMusicState::MatchAction);
		}
		UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] In-game HUD shown"));
	}
}

void ASoccerPlayerController::HideInGameHUD()
{
	if (InGameHUDWidget)
	{
		InGameHUDWidget->RemoveFromParent();
	}

	bShowHUD = false;
	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] In-game HUD hidden"));
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
		if (InGameHUDWidget)
		{
			InGameHUDWidget->RemoveFromParent();
		}

		MainMenuWidget->AddToViewport();
		bShowHUD = false;
		SetShowMouseCursor(true);
		SetInputMode(FInputModeUIOnly());
		if (DynamicMusicComponent)
		{
			DynamicMusicComponent->SetMusicState(ESoccerMusicState::Menu);
		}
		UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] Main menu shown"));
	}
}
void ASoccerPlayerController::SetMusicState(ESoccerMusicState NewState)
{
	if (DynamicMusicComponent)
	{
		DynamicMusicComponent->SetMusicState(NewState);
	}
}
void ASoccerPlayerController::HideMainMenu()
{
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
	}

	ShowInGameHUD();

	bShowHUD = true;
	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());
	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] Main menu hidden"));
}

void ASoccerPlayerController::ShowPlayerCreationScreen()
{
	if (PlayerCreationWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] PlayerCreationWidgetClass not set"));
		return;
	}

	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
	}

	if (InGameHUDWidget)
	{
		InGameHUDWidget->RemoveFromParent();
	}

	if (PlayerCreationWidget == nullptr)
	{
		PlayerCreationWidget = CreateWidget<USoccerPlayerCreationWidget>(this, PlayerCreationWidgetClass);
	}

	if (PlayerCreationWidget)
	{
		PlayerCreationWidget->AddToViewport();
		bShowHUD = false;
		SetShowMouseCursor(true);
		SetInputMode(FInputModeUIOnly());
		if (DynamicMusicComponent)
		{
			DynamicMusicComponent->SetMusicState(ESoccerMusicState::Menu);
		}
		UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] Player creation screen shown"));
	}
}

void ASoccerPlayerController::HidePlayerCreationScreen()
{
	if (PlayerCreationWidget)
	{
		PlayerCreationWidget->RemoveFromParent();
	}

	ShowInGameHUD();

	bShowHUD = true;
	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());
	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] Player creation screen hidden"));
}

void ASoccerPlayerController::ShowSettingsScreen()
{
	if (SettingsWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] SettingsWidgetClass not set"));
		return;
	}

	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
	}

	if (InGameHUDWidget)
	{
		InGameHUDWidget->RemoveFromParent();
	}

	if (SettingsWidget == nullptr)
	{
		SettingsWidget = CreateWidget<USoccerSettingsWidget>(this, SettingsWidgetClass);
	}

	if (SettingsWidget)
	{
		SettingsWidget->AddToViewport();
		bShowHUD = false;
		SetShowMouseCursor(true);
		SetInputMode(FInputModeUIOnly());
		if (DynamicMusicComponent)
		{
			DynamicMusicComponent->SetMusicState(ESoccerMusicState::Menu);
		}
		UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] Settings screen shown"));
	}
}

void ASoccerPlayerController::HideSettingsScreen()
{
	if (SettingsWidget)
	{
		SettingsWidget->RemoveFromParent();
	}

	ShowMainMenu();
	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] Settings screen hidden"));
}

void ASoccerPlayerController::ShowStatisticsScreen()
{
	if (StatisticsWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] StatisticsWidgetClass not set"));
		return;
	}

	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
	}

	if (InGameHUDWidget)
	{
		InGameHUDWidget->RemoveFromParent();
	}

	if (StatisticsWidget == nullptr)
	{
		StatisticsWidget = CreateWidget<USoccerStatisticsWidget>(this, StatisticsWidgetClass);
	}

	if (StatisticsWidget)
	{
		StatisticsWidget->AddToViewport();
		bShowHUD = false;
		SetShowMouseCursor(true);
		SetInputMode(FInputModeUIOnly());
		if (DynamicMusicComponent)
		{
			DynamicMusicComponent->SetMusicState(ESoccerMusicState::Menu);
		}
		UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] Statistics screen shown"));
	}
}

void ASoccerPlayerController::HideStatisticsScreen()
{
	if (StatisticsWidget)
	{
		StatisticsWidget->RemoveFromParent();
	}

	ShowMainMenu();
	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerController] Statistics screen hidden"));
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
