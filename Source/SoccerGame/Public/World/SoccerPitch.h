// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoccerPitch.generated.h"

class UStaticMeshComponent;
class USceneComponent;

/**
 * @class ASoccerPitch
 * @brief Basic futsal pitch and boundary actor for gameplay.
 */
UCLASS()
class SOCCERGAME_API ASoccerPitch : public AActor
{
	GENERATED_BODY()

public:
	ASoccerPitch();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

protected:
	void UpdatePitchGeometry();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Pitch")
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Pitch")
	UStaticMeshComponent* FieldMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Pitch")
	UStaticMeshComponent* LeftBoundary;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Pitch")
	UStaticMeshComponent* RightBoundary;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Pitch")
	UStaticMeshComponent* EndBoundaryFront;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Pitch")
	UStaticMeshComponent* EndBoundaryBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Pitch")
	float FieldLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Pitch")
	float FieldWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Pitch")
	float BoundaryHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Pitch")
	float BoundaryThickness;
};
