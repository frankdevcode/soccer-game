// Copyright Frank Dev Code. All Rights Reserved.

#include "UI/SoccerMainMenuWidget.h"
#include "Core/SoccerPlayerController.h"
#include "Core/SoccerGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Border.h"

void USoccerMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    OwningSoccerController = Cast<ASoccerPlayerController>(GetOwningPlayer());
    SoccerGameMode = OwningSoccerController ? Cast<ASoccerGameMode>(UGameplayStatics::GetGameMode(this)) : nullptr;
}

TSharedRef<SWidget> USoccerMainMenuWidget::RebuildWidget()
{
    RootBox = WidgetTree ? WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("RootBox")) : nullptr;

    if (RootBox == nullptr)
    {
        return Super::RebuildWidget();
    }

    WidgetTree->RootWidget = RootBox;

    const TArray<FString> ButtonLabels = {
        TEXT("Quick Match"),
        TEXT("Career Mode"),
        TEXT("Training Mode"),
        TEXT("Tournament Mode"),
        TEXT("Exit")
    };

    const TArray<FOnClicked> ButtonHandlers = {
        FOnClicked::CreateUObject(this, &USoccerMainMenuWidget::OnQuickMatchClicked),
        FOnClicked::CreateUObject(this, &USoccerMainMenuWidget::OnCareerModeClicked),
        FOnClicked::CreateUObject(this, &USoccerMainMenuWidget::OnTrainingModeClicked),
        FOnClicked::CreateUObject(this, &USoccerMainMenuWidget::OnTournamentModeClicked),
        FOnClicked::CreateUObject(this, &USoccerMainMenuWidget::OnExitClicked)
    };

    for (int32 Index = 0; Index < ButtonLabels.Num(); ++Index)
    {
        UButton* NewButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), *FString::Printf(TEXT("Button_%d"), Index));
        UTextBlock* ButtonText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), *FString::Printf(TEXT("ButtonText_%d"), Index));
        ButtonText->SetText(FText::FromString(ButtonLabels[Index]));
        ButtonText->SetJustification(ETextJustify::Center);
        NewButton->AddChild(ButtonText);
        NewButton->OnClicked.Add(ButtonHandlers[Index]);

        RootBox->AddChildToVerticalBox(NewButton);

        switch (Index)
        {
            case 0: QuickMatchButton = NewButton; break;
            case 1: CareerModeButton = NewButton; break;
            case 2: TrainingModeButton = NewButton; break;
            case 3: TournamentModeButton = NewButton; break;
            case 4: ExitButton = NewButton; break;
            default: break;
        }
    }

    return RootBox->TakeWidget();
}

void USoccerMainMenuWidget::OnQuickMatchClicked()
{
    if (SoccerGameMode)
    {
        SoccerGameMode->StartQuickMatch();
    }

    if (OwningSoccerController)
    {
        OwningSoccerController->HideMainMenu();
    }
}

void USoccerMainMenuWidget::OnCareerModeClicked()
{
    if (SoccerGameMode)
    {
        SoccerGameMode->StartCareerMode();
    }

    if (OwningSoccerController)
    {
        OwningSoccerController->HideMainMenu();
    }
}

void USoccerMainMenuWidget::OnTrainingModeClicked()
{
    if (SoccerGameMode)
    {
        SoccerGameMode->StartTrainingMode();
    }

    if (OwningSoccerController)
    {
        OwningSoccerController->HideMainMenu();
    }
}

void USoccerMainMenuWidget::OnTournamentModeClicked()
{
    if (SoccerGameMode)
    {
        SoccerGameMode->StartTournamentMode();
    }

    if (OwningSoccerController)
    {
        OwningSoccerController->HideMainMenu();
    }
}

void USoccerMainMenuWidget::OnExitClicked()
{
    if (OwningSoccerController)
    {
        OwningSoccerController->ConsoleCommand(TEXT("quit"), true);
    }
}
