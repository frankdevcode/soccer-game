// Copyright Frank Dev Code. All Rights Reserved.

#include "UI/SoccerPlayerCreationWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Core/SoccerPlayerController.h"
#include "Core/SoccerGameMode.h"

void USoccerPlayerCreationWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

TSharedRef<SWidget> USoccerPlayerCreationWidget::RebuildWidget()
{
    UVerticalBox* RootBox = WidgetTree ? WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("PlayerCreationRoot")) : nullptr;
    if (RootBox == nullptr)
    {
        return Super::RebuildWidget();
    }

    WidgetTree->RootWidget = RootBox;

    UBorder* Background = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("CreationBackground"));
    Background->SetPadding(FMargin(16.0f));
    Background->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.5f));

    UVerticalBox* InnerBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("InnerBox"));
    Background->SetContent(InnerBox);

    UTextBlock* TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("TitleText"));
    TitleText->SetText(FText::FromString(TEXT("Create Your Player")));
    TitleText->SetJustification(ETextJustify::Center);
    TitleText->SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 24));
    TitleText->SetColorAndOpacity(FSlateColor(FLinearColor::White));

    NameTextBox = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("NameTextBox"));
    NameTextBox->SetHintText(FText::FromString(TEXT("Enter player name")));

    UHorizontalBox* ButtonRow = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ButtonRow"));

    CreateButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("CreateButton"));
    UTextBlock* CreateButtonText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("CreateButtonText"));
    CreateButtonText->SetText(FText::FromString(TEXT("Create")));
    CreateButtonText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    CreateButton->AddChild(CreateButtonText);
    CreateButton->OnClicked.Add(FOnClicked::CreateUObject(this, &USoccerPlayerCreationWidget::OnCreatePlayerClicked));

    BackButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("BackButton"));
    UTextBlock* BackButtonText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("BackButtonText"));
    BackButtonText->SetText(FText::FromString(TEXT("Back")));
    BackButtonText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    BackButton->AddChild(BackButtonText);
    BackButton->OnClicked.Add(FOnClicked::CreateUObject(this, &USoccerPlayerCreationWidget::OnBackToMenuClicked));

    ButtonRow->AddChildToHorizontalBox(CreateButton);
    ButtonRow->AddChildToHorizontalBox(BackButton);

    InnerBox->AddChildToVerticalBox(TitleText);
    InnerBox->AddChildToVerticalBox(NameTextBox);
    InnerBox->AddChildToVerticalBox(ButtonRow);

    RootBox->AddChildToVerticalBox(Background);

    return RootBox->TakeWidget();
}

void USoccerPlayerCreationWidget::OnCreatePlayerClicked()
{
    if (!NameTextBox)
    {
        return;
    }

    EnteredPlayerName = NameTextBox->GetText().ToString();
    if (EnteredPlayerName.IsEmpty())
    {
        EnteredPlayerName = TEXT("Player");
    }

    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        if (ASoccerPlayerController* SoccerPC = Cast<ASoccerPlayerController>(PC))
        {
            if (ASoccerGameMode* GameMode = Cast<ASoccerGameMode>(UGameplayStatics::GetGameMode(this)))
            {
                GameMode->StartCareerMode(EnteredPlayerName, true);
            }
            SoccerPC->HidePlayerCreationScreen();
        }
    }
}

void USoccerPlayerCreationWidget::OnBackToMenuClicked()
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        if (ASoccerPlayerController* SoccerPC = Cast<ASoccerPlayerController>(PC))
        {
            SoccerPC->HidePlayerCreationScreen();
            SoccerPC->ShowMainMenu();
        }
    }
}