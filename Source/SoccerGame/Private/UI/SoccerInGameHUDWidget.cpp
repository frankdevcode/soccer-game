// Copyright Frank Dev Code. All Rights Reserved.

#include "UI/SoccerInGameHUDWidget.h"
#include "Game/SoccerGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void USoccerInGameHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UpdateHUD();
}

void USoccerInGameHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    UpdateHUD();
}

TSharedRef<SWidget> USoccerInGameHUDWidget::RebuildWidget()
{
    RootBox = WidgetTree ? WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("HUDRootBox")) : nullptr;
    if (RootBox == nullptr)
    {
        return Super::RebuildWidget();
    }

    WidgetTree->RootWidget = RootBox;

    UBorder* Background = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("HUDBackground"));
    Background->SetPadding(FMargin(8.0f));
    Background->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.4f));

    UVerticalBox* InnerBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("InnerBox"));
    Background->SetContent(InnerBox);

    // Score row
    Team1ScoreText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Team1ScoreText"));
    Team1ScoreText->SetText(FText::FromString(TEXT("Team 1: 0")));
    Team1ScoreText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    Team1ScoreText->SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 18));

    Team2ScoreText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Team2ScoreText"));
    Team2ScoreText->SetText(FText::FromString(TEXT("Team 2: 0")));
    Team2ScoreText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    Team2ScoreText->SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 18));

    TimerText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("TimerText"));
    TimerText->SetText(FText::FromString(TEXT("00:00")));
    TimerText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    TimerText->SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20));

    PossessionText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("PossessionText"));
    PossessionText->SetText(FText::FromString(TEXT("Possession: Team 1")));
    PossessionText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    PossessionText->SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 16));

    InnerBox->AddChildToVerticalBox(Team1ScoreText);
    InnerBox->AddChildToVerticalBox(Team2ScoreText);
    InnerBox->AddChildToVerticalBox(TimerText);
    InnerBox->AddChildToVerticalBox(PossessionText);

    return RootBox->TakeWidget();
}

void USoccerInGameHUDWidget::UpdateHUD()
{
    if (ASoccerGameState* GameState = Cast<ASoccerGameState>(UGameplayStatics::GetGameState(this)))
    {
        Team1ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Team 1: %d"), GameState->GetTeamScore(1))));
        Team2ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Team 2: %d"), GameState->GetTeamScore(2))));

        const float Elapsed = GameState->GetMatchElapsedTime();
        const int32 Minutes = FMath::FloorToInt(Elapsed / 60.0f);
        const int32 Seconds = FMath::FloorToInt(FMath::Fmod(Elapsed, 60.0f));
        TimerText->SetText(FText::FromString(FString::Printf(TEXT("Time: %02d:%02d"), Minutes, Seconds)));

        PossessionText->SetText(FText::FromString(FString::Printf(TEXT("Possession: Team %d"), GameState->GetBallPossession())));
    }
}