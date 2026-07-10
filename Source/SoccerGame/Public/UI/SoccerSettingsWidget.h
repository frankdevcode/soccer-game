// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoccerSettingsWidget.generated.h"

class UEditableTextBox;
class UButton;
class UCheckBox;
class USlider;

UCLASS(Blueprintable)
class SOCCERGAME_API USoccerSettingsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual TSharedRef<SWidget> RebuildWidget() override;

    UFUNCTION(BlueprintCallable, Category = "Soccer|Settings")
    void OnSaveSettingsClicked();

    UFUNCTION(BlueprintCallable, Category = "Soccer|Settings")
    void OnResetDefaultsClicked();

    UFUNCTION(BlueprintCallable, Category = "Soccer|Settings")
    void OnBackToMenuClicked();

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Soccer|Settings")
    FString BackendURLValue;

    UPROPERTY(BlueprintReadOnly, Category = "Soccer|Settings")
    int32 BackendPortValue;

    UPROPERTY(BlueprintReadOnly, Category = "Soccer|Settings")
    float MatchDurationValue;

    UPROPERTY(BlueprintReadOnly, Category = "Soccer|Settings")
    float CameraFollowDistanceValue;

    UPROPERTY()
    UEditableTextBox* BackendURLTextBox;

    UPROPERTY()
    UEditableTextBox* BackendPortTextBox;

    UPROPERTY()
    UEditableTextBox* MatchDurationTextBox;

    UPROPERTY()
    UEditableTextBox* CameraFollowDistanceTextBox;

    UPROPERTY()
    UButton* SaveButton;

    UPROPERTY()
    UButton* ResetButton;

    UPROPERTY()
    UButton* BackButton;
};
