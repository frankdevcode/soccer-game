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

	// Don't rotate character with camera
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

	CurrentStamina = MaxStamina;

	UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerCharacter] Player initialized - Team %d, Position: %d"),
		TeamId, static_cast<int32>(PlayerPosition));
}

void ASoccerPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateStamina(DeltaTime);
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

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASoccerPlayerCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASoccerPlayerCharacter::StopSprinting);

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);

		// Kicking
		EnhancedInputComponent->BindAction(KickAction, ETriggerEvent::Triggered, this, &ASoccerPlayerCharacter::KickBall);
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
		}
	}
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
	// TODO: Update animation state based on movement
}
