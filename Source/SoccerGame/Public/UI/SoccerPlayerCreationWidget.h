// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoccerPlayerCreationWidget.generated.h"

class UEditableTextBox;
class UButton;

UCLASS(Blueprintable)
class SOCCERGAME_API USoccerPlayerCreationWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual TSharedRef<SWidget> RebuildWidget() override;

    UFUNCTION(BlueprintCallable, Category = "Soccer|Career")
    void OnCreatePlayerClicked();

    UFUNCTION(BlueprintCallable, Category = "Soccer|Career")
    void OnBackToMenuClicked();

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Soccer|Career")
    FString EnteredPlayerName;

    UPROPERTY()
    UEditableTextBox* NameTextBox;

    UPROPERTY()
    UButton* CreateButton;

    UPROPERTY()
    UButton* BackButton;
};