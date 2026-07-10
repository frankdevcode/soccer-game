// Copyright Frank Dev Code. All Rights Reserved.

#include "UI/SoccerStatisticsWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Core/SoccerPlayerController.h"
#include "Game/SoccerGameState.h"

void USoccerStatisticsWidget::NativeConstruct()
{
    Super::NativeConstruct();
    RefreshStatistics();
}

void USoccerStatisticsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    RefreshStatistics();
}

TSharedRef<SWidget> USoccerStatisticsWidget::RebuildWidget()
{
    UVerticalBox* RootBox = WidgetTree ? WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("StatisticsRoot")) : nullptr;
    if (RootBox == nullptr)
    {
        return Super::RebuildWidget();
    }

    WidgetTree->RootWidget = RootBox;

    UBorder* Background = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("StatisticsBackground"));
    Background->SetPadding(FMargin(16.0f));
    Background->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.65f));

    UVerticalBox* InnerBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("InnerBox"));
    Background->SetContent(InnerBox);

    UTextBlock* TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("TitleText"));
    TitleText->SetText(FText::FromString(TEXT("Match Statistics & Analytics")));
    TitleText->SetJustification(ETextJustify::Center);
    TitleText->SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 24));
    TitleText->SetColorAndOpacity(FSlateColor(FLinearColor::White));

    Team1StatsText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Team1StatsText"));
    Team1StatsText->SetText(FText::FromString(TEXT("Team 1 - Score: 0 | Fouls: 0 | Possession: 0%")));
    Team1StatsText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    Team1StatsText->SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 18));

    Team2StatsText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Team2StatsText"));
    Team2StatsText->SetText(FText::FromString(TEXT("Team 2 - Score: 0 | Fouls: 0 | Possession: 0%")));
    Team2StatsText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    Team2StatsText->SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 18));

    MatchTimeText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("MatchTimeText"));
    MatchTimeText->SetText(FText::FromString(TEXT("Elapsed Time: 00:00")));
    MatchTimeText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    MatchTimeText->SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 18));

    RegisteredPlayersText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("RegisteredPlayersText"));
    RegisteredPlayersText->SetText(FText::FromString(TEXT("Registered Players: 0")));
    RegisteredPlayersText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    RegisteredPlayersText->SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 18));

    BackButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("BackButton"));
    UTextBlock* BackButtonText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("BackButtonText"));
    BackButtonText->SetText(FText::FromString(TEXT("Back")));
    BackButtonText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    BackButton->AddChild(BackButtonText);
    BackButton->OnClicked.Add(FOnClicked::CreateUObject(this, &USoccerStatisticsWidget::OnBackToMenuClicked));

    InnerBox->AddChildToVerticalBox(TitleText);
    InnerBox->AddChildToVerticalBox(Team1StatsText);
    InnerBox->AddChildToVerticalBox(Team2StatsText);
    InnerBox->AddChildToVerticalBox(MatchTimeText);
    InnerBox->AddChildToVerticalBox(RegisteredPlayersText);
    InnerBox->AddChildToVerticalBox(BackButton);

    RootBox->AddChildToVerticalBox(Background);

    return RootBox->TakeWidget();
}

void USoccerStatisticsWidget::RefreshStatistics()
{
    ASoccerGameState* GameState = GetWorld() ? GetWorld()->GetGameState<ASoccerGameState>() : nullptr;
    if (!GameState)
    {
        return;
    }

    const FTeamStats Team1Stats = GameState->GetTeamStats(1);
    const FTeamStats Team2Stats = GameState->GetTeamStats(2);
    const float ElapsedTime = GameState->GetMatchElapsedTime();

    const int32 Team1Possession = Team1Stats.Possession;
    const int32 Team2Possession = Team2Stats.Possession;

    if (Team1StatsText)
    {
        Team1StatsText->SetText(FText::FromString(FString::Printf(TEXT("Team 1 - Score: %d | Fouls: %d | Possession: %d%%"), Team1Stats.Score, Team1Stats.Fouls, Team1Possession)));
    }

    if (Team2StatsText)
    {
        Team2StatsText->SetText(FText::FromString(FString::Printf(TEXT("Team 2 - Score: %d | Fouls: %d | Possession: %d%%"), Team2Stats.Score, Team2Stats.Fouls, Team2Possession)));
    }

    if (MatchTimeText)
    {
        const int32 Minutes = FMath::FloorToInt(ElapsedTime / 60.0f);
        const int32 Seconds = FMath::FloorToInt(FMath::Fmod(ElapsedTime, 60.0f));
        MatchTimeText->SetText(FText::FromString(FString::Printf(TEXT("Elapsed Time: %02d:%02d"), Minutes, Seconds)));
    }

    if (RegisteredPlayersText)
    {
        RegisteredPlayersText->SetText(FText::FromString(FString::Printf(TEXT("Registered Players: %d"), GameState->GetPlayerCount())));
    }
}

void USoccerStatisticsWidget::OnBackToMenuClicked()
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        if (ASoccerPlayerController* SoccerPC = Cast<ASoccerPlayerController>(PC))
        {
            SoccerPC->HideStatisticsScreen();
            SoccerPC->ShowMainMenu();
        }
    }
}
