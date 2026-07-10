// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "SoccerMainMenuWidget.generated.h"

class ASoccerPlayerController;
class ASoccerGameMode;

UCLASS(Blueprintable)
class SOCCERGAME_API USoccerMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual TSharedRef<SWidget> RebuildWidget() override;

    UFUNCTION(BlueprintCallable, Category = "Soccer|Menu")
    void OnQuickMatchClicked();

    UFUNCTION(BlueprintCallable, Category = "Soccer|Menu")
    void OnCareerModeClicked();

    UFUNCTION(BlueprintCallable, Category = "Soccer|Menu")
    void OnTrainingModeClicked();

    UFUNCTION(BlueprintCallable, Category = "Soccer|Menu")
    void OnTournamentModeClicked();

    UFUNCTION(BlueprintCallable, Category = "Soccer|Menu")
    void OnExitClicked();

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Soccer|Menu")
    ASoccerPlayerController* OwningSoccerController;

    UPROPERTY(BlueprintReadOnly, Category = "Soccer|Menu")
    ASoccerGameMode* SoccerGameMode;

private:
    UPROPERTY()
    UVerticalBox* RootBox;

    UPROPERTY()
    UButton* QuickMatchButton;

    UPROPERTY()
    UButton* CareerModeButton;

    UPROPERTY()
    UButton* TrainingModeButton;

    UPROPERTY()
    UButton* TournamentModeButton;

    UPROPERTY()
    UButton* ExitButton;
};