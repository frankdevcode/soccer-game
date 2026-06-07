// Copyright Frank Dev Code. All Rights Reserved.

#include "Characters/SoccerPlayerCharacter.h"
#include "Camera/CameraBoom.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputModifiers.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Ball/SoccerBall.h"
#include "Characters/SoccerCharacterRig.h"
#include "Characters/SoccerPlayerAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Core/SoccerGameSettings.h"

ASoccerPlayerCharacter::ASoccerPlayerCharacter()
	: TeamId(1)
	, PlayerPosition(EPlayerPosition::Midfielder)
	, PlayerNumber(1)
	, MaxWalkSpeed(600.0f)
	, SprintSpeedMultiplier(1.5f)
	, Acceleration(1200.0f)
	, Deceleration(1600.0f)
	, bIsSprinting(false)
	, BallInteractionRange(300.0f)
	, GoalkeeperSaveRange(1200.0f)
	, GoalkeeperDiveForce(650.0f)
	, GoalkeeperDiveCooldown(1.8f)
	, GoalkeeperDiveStaminaCost(15.0f)
	, GoalkeeperPushbackStrength(850.0f)
	, bGoalkeeperCanDive(true)
	, LastGoalkeeperDiveTime(-1000.0f)
	, CharacterRigDefinition(nullptr)
	, DefaultSkeletalMesh(nullptr)
	, DefaultAnimInstanceClass(nullptr)
	, CharacterHeight(1.0f)
	, CharacterWeight(1.0f)
	, CameraFollowDistance(400.0f)
	, MinCameraDistance(250.0f)
	, MaxCameraDistance(800.0f)
	, CameraZoomStep(50.0f)
	, CameraPitchMin(-60.0f)
	, CameraPitchMax(20.0f)
	, CameraLagSpeed(12.0f)
	, CameraRotationLagSpeed(12.0f)
	, MaxStamina(100.0f)
	, CurrentStamina(100.0f)
	, StaminaRegenerationRate(20.0f)
	, SprintStaminaCost(30.0f)
	, DesiredRotationSpeed(500.0f)
	, MovementInput(FVector2D::ZeroVector)
{
	// Don't rotate with camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;
	GetCharacterMovement()->MaxAcceleration = Acceleration;
	GetCharacterMovement()->BrakingDecelerationWalking = Deceleration;

	// Camera system
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = CameraFollowDistance;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = CameraLagSpeed;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraRotationLagSpeed = CameraRotationLagSpeed;
	CameraBoom->ProbeSize = 12.0f;
	CameraBoom->bDoCollisionTest = true;

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create camera boom (pulls toward player if there's a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	PrimaryActorTick.TickInterval = 0.016f; // ~60 FPS
}

void ASoccerPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
			PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (USoccerGameSettings* Settings = USoccerGameSettings::Get())
	{
		CameraFollowDistance = Settings->CameraFollowDistance;
		MinCameraDistance = Settings->CameraMinDistance;
		MaxCameraDistance = Settings->CameraMaxDistance;
		CameraZoomStep = Settings->CameraZoomStep;
		CameraPitchMin = Settings->CameraPitchMin;
		CameraPitchMax = Settings->CameraPitchMax;
		CameraLagSpeed = Settings->CameraLagSpeed;
		CameraRotationLagSpeed = Settings->CameraRotationLagSpeed;

		if (CameraBoom)
		{
			CameraBoom->TargetArmLength = CameraFollowDistance;
			CameraBoom->CameraLagSpeed = CameraLagSpeed;
			CameraBoom->CameraRotationLagSpeed = CameraRotationLagSpeed;
		}
	}

	SetupCharacterAppearance();
	CurrentStamina = MaxStamina;

	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerCharacter] Player initialized - Team %d, Position: %d"),
		TeamId, static_cast<int32>(PlayerPosition));
}

void ASoccerPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateStamina(DeltaTime);
	UpdateGoalkeeperState(DeltaTime);
	UpdateAnimations();
}

void ASoccerPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = 
		Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoccerPlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASoccerPlayerCharacter::Look);

		// Zoom
		if (ZoomAction)
		{
			EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ASoccerPlayerCharacter::ZoomCamera);
		}

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASoccerPlayerCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASoccerPlayerCharacter::StopSprinting);

		// Jumping / goalkeeper save
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASoccerPlayerCharacter::JumpForBall);

		// Kicking
		EnhancedInputComponent->BindAction(KickAction, ETriggerEvent::Triggered, this, &ASoccerPlayerCharacter::KickBall);

		// Goalkeeper save / dive
		if (SaveAction)
		{
			EnhancedInputComponent->BindAction(SaveAction, ETriggerEvent::Started, this, &ASoccerPlayerCharacter::DiveForSave);
		}
	}
}

void ASoccerPlayerCharacter::InitializePlayer(int32 InTeamId, EPlayerPosition InPosition, int32 InPlayerNumber)
{
	TeamId = InTeamId;
	PlayerPosition = InPosition;
	PlayerNumber = InPlayerNumber;

	// Set team color (visual distinction)
	// TODO: Apply team-specific materials/skins

	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerCharacter] Initialized as Team %d #%d - Position %d"),
		TeamId, PlayerNumber, static_cast<int32>(PlayerPosition));
}

void ASoccerPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (Controller == nullptr)
	{
		return;
	}

	MovementInput = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	const FVector Forward = FRotMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector DesiredMovement = (Forward * MovementInput.Y) + (Right * MovementInput.X);

	if (!DesiredMovement.IsNearlyZero())
	{
		AddMovementInput(DesiredMovement.GetSafeNormal(), 1.0f);
		LastMovementInput = DesiredMovement;
		return;
	}

	LastMovementInput = FVector::ZeroVector;
}

void ASoccerPlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		LastMovementInput = Direction * Value;
	}
}

void ASoccerPlayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);

		LastMovementInput = Direction * Value;
	}
}

void ASoccerPlayerCharacter::Look(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D LookAxisVector = Value.Get<FVector2D>();

		if (LookAxisVector.X != 0.0f)
		{
			AddControllerYawInput(LookAxisVector.X);
		}
		if (LookAxisVector.Y != 0.0f)
		{
			AddControllerPitchInput(LookAxisVector.Y);
			FRotator ControlRotation = Controller->GetControlRotation();
			ControlRotation.Pitch = FMath::Clamp(ControlRotation.Pitch, CameraPitchMin, CameraPitchMax);
			Controller->SetControlRotation(ControlRotation);
		}
	}
}

void ASoccerPlayerCharacter::ZoomCamera(const FInputActionValue& Value)
{
	if (!CameraBoom)
	{
		return;
	}

	const float ZoomValue = Value.Get<float>();
	if (FMath::IsNearlyZero(ZoomValue))
	{
		return;
	}

	const float NewArmLength = FMath::Clamp(CameraBoom->TargetArmLength + ZoomValue * CameraZoomStep, MinCameraDistance, MaxCameraDistance);
	CameraBoom->TargetArmLength = NewArmLength;
}

void ASoccerPlayerCharacter::Sprint()
{
	if (CurrentStamina > 0)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed * SprintSpeedMultiplier;
	}
}

void ASoccerPlayerCharacter::StopSprinting()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

void ASoccerPlayerCharacter::KickBall(float Power)
{
	if (CurrentStamina >= 5.0f)
	{
		ASoccerBall* Ball = FindNearestBall(BallInteractionRange);
		if (Ball)
		{
			CurrentStamina -= 5.0f;
			const FVector BallDirection = (Ball->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			Ball->ApplyKick(BallDirection, Power);
			UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerCharacter] Kicking ball with power %.1f"), Power);
		}
	}
}

void ASoccerPlayerCharacter::PassBall(AActor* TargetPlayer, float Power)
{
	if (CurrentStamina >= 3.0f && TargetPlayer)
	{
		ASoccerBall* Ball = FindNearestBall(BallInteractionRange);
		if (Ball)
		{
			CurrentStamina -= 3.0f;
			const FVector BallDirection = (TargetPlayer->GetActorLocation() - Ball->GetActorLocation()).GetSafeNormal();
			Ball->ApplyPass(BallDirection, Power);
			UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerCharacter] Passing ball to teammate with power %.1f"), Power);
		}
	}
}

void ASoccerPlayerCharacter::HeadBall()
{
	if (CurrentStamina >= 2.0f)
	{
		ASoccerBall* Ball = FindNearestBall(BallInteractionRange);
		if (Ball)
		{
			CurrentStamina -= 2.0f;
			const FVector BallDirection = (Ball->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			Ball->ApplyHeader(BallDirection, 1.0f);
			UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerCharacter] Heading ball"));
		}
	}
}

void ASoccerPlayerCharacter::JumpForBall()
{
	if (PlayerPosition == EPlayerPosition::Goalkeeper)
	{
		DiveForSave();
		return;
	}

	if (CurrentStamina >= 10.0f)
	{
		CurrentStamina -= 10.0f;
		Jump();
		UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerCharacter] Jumping for ball"));
	}
}

ASoccerBall* ASoccerPlayerCharacter::FindNearestBall(float MaxDistance) const
{
	if (!GetWorld())
	{
		return nullptr;
	}

	TArray<AActor*> FoundBalls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoccerBall::StaticClass(), FoundBalls);

	ASoccerBall* ClosestBall = nullptr;
	float ClosestDistance = MaxDistance;

	for (AActor* Actor : FoundBalls)
	{
		ASoccerBall* Ball = Cast<ASoccerBall>(Actor);
		if (!Ball)
		{
			continue;
		}

		const float Distance = FVector::Dist(Ball->GetActorLocation(), GetActorLocation());
		if (Distance <= ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestBall = Ball;
		}
	}

	return ClosestBall;
}

void ASoccerPlayerCharacter::ConfigurePlayerModel(USkeletalMesh* Mesh, TSubclassOf<UAnimInstance> AnimBP)
{
	if (Mesh)
	{
		DefaultSkeletalMesh = Mesh;
	}

	if (AnimBP)
	{
		DefaultAnimInstanceClass = AnimBP;
	}

	ApplyCharacterRig();
}

void ASoccerPlayerCharacter::ApplyCharacterRig()
{
	if (CharacterRigDefinition)
	{
		CharacterRigDefinition->ApplyToCharacter(this);
		return;
	}

	USkeletalMeshComponent* MeshComponent = GetMesh();
	if (!MeshComponent)
	{
		return;
	}

	if (DefaultSkeletalMesh)
	{
		MeshComponent->SetSkeletalMesh(DefaultSkeletalMesh);
	}

	if (DefaultAnimInstanceClass)
	{
		MeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		MeshComponent->SetAnimInstanceClass(DefaultAnimInstanceClass);
	}

	MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	MeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	MeshComponent->SetRelativeScale3D(FVector(CharacterHeight, CharacterWeight, CharacterHeight));
}

void ASoccerPlayerCharacter::SetupCharacterAppearance()
{
	if (CameraBoom)
	{
		CameraBoom->TargetArmLength = CameraFollowDistance;
		CameraBoom->CameraLagSpeed = CameraLagSpeed;
		CameraBoom->CameraRotationLagSpeed = CameraRotationLagSpeed;
	}

	ApplyCharacterRig();
}

void ASoccerPlayerCharacter::UpdateGoalkeeperState(float DeltaTime)
{
	if (PlayerPosition != EPlayerPosition::Goalkeeper)
	{
		return;
	}

	const float CurrentTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	bGoalkeeperCanDive = CurrentTime >= LastGoalkeeperDiveTime + GoalkeeperDiveCooldown;

	if (CurrentStamina < GoalkeeperDiveStaminaCost)
	{
		bGoalkeeperCanDive = false;
	}
}

void ASoccerPlayerCharacter::DiveForSave()
{
	if (PlayerPosition != EPlayerPosition::Goalkeeper || !bGoalkeeperCanDive)
	{
		return;
	}

	ASoccerBall* Ball = FindNearestBall(GoalkeeperSaveRange);
	if (!Ball)
	{
		return;
	}

	const FVector BallDirection = (Ball->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	LaunchCharacter(FVector(BallDirection.X * GoalkeeperPushbackStrength, BallDirection.Y * GoalkeeperPushbackStrength, GoalkeeperDiveForce), true, true);

	CurrentStamina = FMath::Max(0.0f, CurrentStamina - GoalkeeperDiveStaminaCost);
	LastGoalkeeperDiveTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	bGoalkeeperCanDive = false;

	AttemptGoalkeeperSave();

	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerCharacter] Goalkeeper dive for save"));
}

void ASoccerPlayerCharacter::AttemptGoalkeeperSave()
{
	if (PlayerPosition != EPlayerPosition::Goalkeeper)
	{
		return;
	}

	ASoccerBall* Ball = FindNearestBall(GoalkeeperSaveRange);
	if (!Ball)
	{
		return;
	}

	const FVector ToBall = Ball->GetActorLocation() - GetActorLocation();
	const float Distance = ToBall.Size();
	if (Distance > GoalkeeperSaveRange)
	{
		return;
	}

	const FVector SaveDirection = ToBall.GetSafeNormal();
	Ball->ApplyKick(SaveDirection * -1.0f, 0.85f);
	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerCharacter] Goalkeeper attempted save"));
}

bool ASoccerPlayerCharacter::IsInGoalkeeperZone() const
{
	if (PlayerPosition != EPlayerPosition::Goalkeeper)
	{
		return false;
	}

	if (USoccerGameSettings* Settings = USoccerGameSettings::Get())
	{
		const float FieldHalfLength = Settings->FieldLength * 0.5f;
		const float HalfGoalWidth = Settings->GoalWidth * 0.5f;
		const float GoalDepth = Settings->GoalDepth;
		const FVector Location = GetActorLocation();

		if (TeamId == 1)
		{
			return Location.Y <= -FieldHalfLength + GoalDepth && FMath::Abs(Location.X) <= HalfGoalWidth;
		}
		return Location.Y >= FieldHalfLength - GoalDepth && FMath::Abs(Location.X) <= HalfGoalWidth;
	}

	return false;
}

void ASoccerPlayerCharacter::UpdateStamina(float DeltaTime)
{
	if (bIsSprinting)
	{
		ConsumeSprint(DeltaTime);
	}
	else
	{
		RegenerateStamina(DeltaTime);
	}

	CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);
}

void ASoccerPlayerCharacter::RegenerateStamina(float DeltaTime)
{
	CurrentStamina += StaminaRegenerationRate * DeltaTime;
}

void ASoccerPlayerCharacter::ConsumeSprint(float DeltaTime)
{
	CurrentStamina -= SprintStaminaCost * DeltaTime;

	if (CurrentStamina <= 0.0f)
	{
		StopSprinting();
	}
}

void ASoccerPlayerCharacter::UpdateAnimations()
{
	USkeletalMeshComponent* MeshComponent = GetMesh();
	if (!MeshComponent)
	{
		return;
	}

	if (USoccerPlayerAnimInstance* AnimInstance = Cast<USoccerPlayerAnimInstance>(MeshComponent->GetAnimInstance()))
	{
		const FVector Velocity = GetVelocity();
		const float Speed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();
		const bool bInAir = GetCharacterMovement() ? GetCharacterMovement()->IsFalling() : false;
		const bool bMoving = Speed > KINDA_SMALL_NUMBER;
		const FVector LocalVelocity = GetActorRotation().UnrotateVector(Velocity);
		const float Direction = LocalVelocity.IsNearlyZero() ? 0.0f : FMath::RadiansToDegrees(FMath::Atan2(LocalVelocity.Y, LocalVelocity.X));

		AnimInstance->UpdateState(Speed, Direction, bMoving, bIsSprinting, bInAir);
	}
}
