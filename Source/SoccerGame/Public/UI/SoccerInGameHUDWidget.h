// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoccerInGameHUDWidget.generated.h"

class UVerticalBox;
class UTextBlock;

UCLASS(Blueprintable)
class SOCCERGAME_API USoccerInGameHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    virtual TSharedRef<SWidget> RebuildWidget() override;

protected:
    UPROPERTY()
    UVerticalBox* RootBox;

    UPROPERTY()
    UTextBlock* Team1ScoreText;

    UPROPERTY()
    UTextBlock* Team2ScoreText;

    UPROPERTY()
    UTextBlock* TimerText;

    UPROPERTY()
    UTextBlock* PossessionText;

private:
    void UpdateHUD();
};