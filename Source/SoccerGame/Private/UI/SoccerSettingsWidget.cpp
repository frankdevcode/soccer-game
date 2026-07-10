// Copyright Frank Dev Code. All Rights Reserved.

#include "UI/SoccerSettingsWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Core/SoccerPlayerController.h"
#include "Core/SoccerGameSettings.h"

void USoccerSettingsWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

TSharedRef<SWidget> USoccerSettingsWidget::RebuildWidget()
{
    UVerticalBox* RootBox = WidgetTree ? WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("SettingsRoot")) : nullptr;
    if (RootBox == nullptr)
    {
        return Super::RebuildWidget();
    }

    WidgetTree->RootWidget = RootBox;

    UBorder* Background = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("SettingsBackground"));
    Background->SetPadding(FMargin(16.0f));
    Background->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.65f));

    UVerticalBox* InnerBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("InnerBox"));
    Background->SetContent(InnerBox);

    UTextBlock* TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("TitleText"));
    TitleText->SetText(FText::FromString(TEXT("Settings & Configuration")));
    TitleText->SetJustification(ETextJustify::Center);
    TitleText->SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 24));
    TitleText->SetColorAndOpacity(FSlateColor(FLinearColor::White));

    UTextBlock* BackendURLLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("BackendURLLabel"));
    BackendURLLabel->SetText(FText::FromString(TEXT("Backend URL")));
    BackendURLLabel->SetColorAndOpacity(FSlateColor(FLinearColor::White));

    BackendURLTextBox = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("BackendURLTextBox"));
    BackendURLTextBox->SetHintText(FText::FromString(TEXT("localhost")));

    UTextBlock* BackendPortLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("BackendPortLabel"));
    BackendPortLabel->SetText(FText::FromString(TEXT("Backend Port")));
    BackendPortLabel->SetColorAndOpacity(FSlateColor(FLinearColor::White));

    BackendPortTextBox = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("BackendPortTextBox"));
    BackendPortTextBox->SetHintText(FText::FromString(TEXT("3001")));

    UTextBlock* MatchDurationLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("MatchDurationLabel"));
    MatchDurationLabel->SetText(FText::FromString(TEXT("Match Duration (sec)")));
    MatchDurationLabel->SetColorAndOpacity(FSlateColor(FLinearColor::White));

    MatchDurationTextBox = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("MatchDurationTextBox"));
    MatchDurationTextBox->SetHintText(FText::FromString(TEXT("1200")));

    UTextBlock* CameraFollowDistanceLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("CameraFollowDistanceLabel"));
    CameraFollowDistanceLabel->SetText(FText::FromString(TEXT("Camera Follow Distance")));
    CameraFollowDistanceLabel->SetColorAndOpacity(FSlateColor(FLinearColor::White));

    CameraFollowDistanceTextBox = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("CameraFollowDistanceTextBox"));
    CameraFollowDistanceTextBox->SetHintText(FText::FromString(TEXT("400")));

    SaveButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("SaveButton"));
    UTextBlock* SaveButtonText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SaveButtonText"));
    SaveButtonText->SetText(FText::FromString(TEXT("Save Settings")));
    SaveButtonText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    SaveButton->AddChild(SaveButtonText);
    SaveButton->OnClicked.Add(FOnClicked::CreateUObject(this, &USoccerSettingsWidget::OnSaveSettingsClicked));

    ResetButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ResetButton"));
    UTextBlock* ResetButtonText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ResetButtonText"));
    ResetButtonText->SetText(FText::FromString(TEXT("Reset Defaults")));
    ResetButtonText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    ResetButton->AddChild(ResetButtonText);
    ResetButton->OnClicked.Add(FOnClicked::CreateUObject(this, &USoccerSettingsWidget::OnResetDefaultsClicked));

    BackButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("BackButton"));
    UTextBlock* BackButtonText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("BackButtonText"));
    BackButtonText->SetText(FText::FromString(TEXT("Back")));
    BackButtonText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    BackButton->AddChild(BackButtonText);
    BackButton->OnClicked.Add(FOnClicked::CreateUObject(this, &USoccerSettingsWidget::OnBackToMenuClicked));

    UHorizontalBox* ButtonRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ButtonRow"));
    ButtonRow->AddChildToHorizontalBox(SaveButton);
    ButtonRow->AddChildToHorizontalBox(ResetButton);
    ButtonRow->AddChildToHorizontalBox(BackButton);

    InnerBox->AddChildToVerticalBox(TitleText);
    InnerBox->AddChildToVerticalBox(BackendURLLabel);
    InnerBox->AddChildToVerticalBox(BackendURLTextBox);
    InnerBox->AddChildToVerticalBox(BackendPortLabel);
    InnerBox->AddChildToVerticalBox(BackendPortTextBox);
    InnerBox->AddChildToVerticalBox(MatchDurationLabel);
    InnerBox->AddChildToVerticalBox(MatchDurationTextBox);
    InnerBox->AddChildToVerticalBox(CameraFollowDistanceLabel);
    InnerBox->AddChildToVerticalBox(CameraFollowDistanceTextBox);
    InnerBox->AddChildToVerticalBox(ButtonRow);

    RootBox->AddChildToVerticalBox(Background);

    return RootBox->TakeWidget();
}

void USoccerSettingsWidget::OnSaveSettingsClicked()
{
    USoccerGameSettings* Settings = USoccerGameSettings::Get();
    if (!Settings)
    {
        return;
    }

    if (BackendURLTextBox)
    {
        Settings->BackendURL = BackendURLTextBox->GetText().ToString();
    }

    if (BackendPortTextBox)
    {
        BackendPortValue = FCString::Atoi(*BackendPortTextBox->GetText().ToString());
        Settings->BackendPort = BackendPortValue;
    }

    if (MatchDurationTextBox)
    {
        MatchDurationValue = FCString::Atof(*MatchDurationTextBox->GetText().ToString());
        Settings->MatchDuration = MatchDurationValue;
    }

    if (CameraFollowDistanceTextBox)
    {
        CameraFollowDistanceValue = FCString::Atof(*CameraFollowDistanceTextBox->GetText().ToString());
        Settings->CameraFollowDistance = CameraFollowDistanceValue;
    }

    Settings->SaveSettings();
    UE_LOG(LogTemp, Warning, TEXT("[SoccerSettingsWidget] Settings saved"));
}

void USoccerSettingsWidget::OnResetDefaultsClicked()
{
    USoccerGameSettings* Settings = USoccerGameSettings::Get();
    if (!Settings)
    {
        return;
    }

    Settings->ResetToDefaults();
    Settings->SaveSettings();

    if (BackendURLTextBox)
    {
        BackendURLTextBox->SetText(FText::FromString(Settings->BackendURL));
    }
    if (BackendPortTextBox)
    {
        BackendPortTextBox->SetText(FText::AsNumber(Settings->BackendPort));
    }
    if (MatchDurationTextBox)
    {
        MatchDurationTextBox->SetText(FText::AsNumber(Settings->MatchDuration));
    }
    if (CameraFollowDistanceTextBox)
    {
        CameraFollowDistanceTextBox->SetText(FText::AsNumber(Settings->CameraFollowDistance));
    }

    UE_LOG(LogTemp, Warning, TEXT("[SoccerSettingsWidget] Settings reset to defaults"));
}

void USoccerSettingsWidget::OnBackToMenuClicked()
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        if (ASoccerPlayerController* SoccerPC = Cast<ASoccerPlayerController>(PC))
        {
            SoccerPC->HideSettingsScreen();
            SoccerPC->ShowMainMenu();
        }
    }
}
