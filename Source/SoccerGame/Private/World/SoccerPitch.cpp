// Copyright Frank Dev Code. All Rights Reserved.

#include "World/SoccerPitch.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Core/SoccerGameSettings.h"

ASoccerPitch::ASoccerPitch()
	: FieldLength(4000.0f)
	, FieldWidth(2000.0f)
	, BoundaryHeight(120.0f)
	, BoundaryThickness(30.0f)
{
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	FieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FieldMesh"));
	FieldMesh->SetupAttachment(RootComponent);
	FieldMesh->SetCollisionProfileName(TEXT("BlockAll"));

	LeftBoundary = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftBoundary"));
	LeftBoundary->SetupAttachment(RootComponent);
	LeftBoundary->SetCollisionProfileName(TEXT("BlockAll"));

	RightBoundary = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightBoundary"));
	RightBoundary->SetupAttachment(RootComponent);
	RightBoundary->SetCollisionProfileName(TEXT("BlockAll"));

	EndBoundaryFront = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndBoundaryFront"));
	EndBoundaryFront->SetupAttachment(RootComponent);
	EndBoundaryFront->SetCollisionProfileName(TEXT("BlockAll"));

	EndBoundaryBack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndBoundaryBack"));
	EndBoundaryBack->SetupAttachment(RootComponent);
	EndBoundaryBack->SetCollisionProfileName(TEXT("BlockAll"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	if (PlaneAsset.Succeeded())
	{
		FieldMesh->SetStaticMesh(PlaneAsset.Object);
	}
	if (CubeAsset.Succeeded())
	{
		LeftBoundary->SetStaticMesh(CubeAsset.Object);
		RightBoundary->SetStaticMesh(CubeAsset.Object);
		EndBoundaryFront->SetStaticMesh(CubeAsset.Object);
		EndBoundaryBack->SetStaticMesh(CubeAsset.Object);
	}
}

void ASoccerPitch::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdatePitchGeometry();
}

void ASoccerPitch::BeginPlay()
{
	Super::BeginPlay();

	if (USoccerGameSettings* Settings = USoccerGameSettings::Get())
	{
		FieldLength = Settings->FieldLength;
		FieldWidth = Settings->FieldWidth;
		BoundaryHeight = Settings->BoundaryHeight;
		BoundaryThickness = Settings->BoundaryThickness;
	}

	UpdatePitchGeometry();
}

void ASoccerPitch::UpdatePitchGeometry()
{
	const float PlaneSizeCm = 100.0f;
	const FVector FieldScale(FieldWidth / PlaneSizeCm, FieldLength / PlaneSizeCm, 1.0f);
	FieldMesh->SetRelativeScale3D(FieldScale);

	const float BoundaryHalfWidth = BoundaryThickness * 0.5f;
	const float BoundaryZ = BoundaryHeight * 0.5f;

	LeftBoundary->SetRelativeScale3D(FVector(BoundaryThickness / PlaneSizeCm, FieldLength / PlaneSizeCm, BoundaryHeight / PlaneSizeCm));
	LeftBoundary->SetRelativeLocation(FVector(-FieldWidth * 0.5f - BoundaryHalfWidth, 0.0f, BoundaryZ));

	RightBoundary->SetRelativeScale3D(FVector(BoundaryThickness / PlaneSizeCm, FieldLength / PlaneSizeCm, BoundaryHeight / PlaneSizeCm));
	RightBoundary->SetRelativeLocation(FVector(FieldWidth * 0.5f + BoundaryHalfWidth, 0.0f, BoundaryZ));

	EndBoundaryFront->SetRelativeScale3D(FVector(FieldWidth / PlaneSizeCm, BoundaryThickness / PlaneSizeCm, BoundaryHeight / PlaneSizeCm));
	EndBoundaryFront->SetRelativeLocation(FVector(0.0f, FieldLength * 0.5f + BoundaryHalfWidth, BoundaryZ));

	EndBoundaryBack->SetRelativeScale3D(FVector(FieldWidth / PlaneSizeCm, BoundaryThickness / PlaneSizeCm, BoundaryHeight / PlaneSizeCm));
	EndBoundaryBack->SetRelativeLocation(FVector(0.0f, -FieldLength * 0.5f - BoundaryHalfWidth, BoundaryZ));
}
