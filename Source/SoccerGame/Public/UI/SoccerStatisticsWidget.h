// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoccerStatisticsWidget.generated.h"

class UTextBlock;
class UButton;
class UVerticalBox;

UCLASS(Blueprintable)
class SOCCERGAME_API USoccerStatisticsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    virtual TSharedRef<SWidget> RebuildWidget() override;

    UFUNCTION(BlueprintCallable, Category = "Soccer|Analytics")
    void OnBackToMenuClicked();

protected:
    void RefreshStatistics();

    UPROPERTY()
    UTextBlock* Team1StatsText;

    UPROPERTY()
    UTextBlock* Team2StatsText;

    UPROPERTY()
    UTextBlock* MatchTimeText;

    UPROPERTY()
    UTextBlock* RegisteredPlayersText;

    UPROPERTY()
    UButton* BackButton;
};
