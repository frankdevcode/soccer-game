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

ASoccerPlayerCharacter::ASoccerPlayerCharacter()
	: TeamId(1)
	, PlayerPosition(EPlayerPosition::Midfielder)
	, PlayerNumber(1)
	, MaxWalkSpeed(600.0f)
	, SprintSpeedMultiplier(1.5f)
	, bIsSprinting(false)
	, MaxStamina(100.0f)
	, CurrentStamina(100.0f)
	, StaminaRegenerationRate(20.0f)
	, SprintStaminaCost(30.0f)
	, DesiredRotationSpeed(500.0f)
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoccerPlayerCharacter::MoveForward);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoccerPlayerCharacter::MoveRight);

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

void ASoccerPlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// Find forward direction
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
		// Find right direction
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
		CurrentStamina -= 5.0f;
		UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerCharacter] Kicking ball with power %.1f"), Power);
		// TODO: Implement ball physics interaction
	}
}

void ASoccerPlayerCharacter::PassBall(AActor* TargetPlayer, float Power)
{
	if (CurrentStamina >= 3.0f && TargetPlayer)
	{
		CurrentStamina -= 3.0f;
		UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerCharacter] Passing ball to teammate with power %.1f"), Power);
		// TODO: Implement pass mechanics
	}
}

void ASoccerPlayerCharacter::HeadBall()
{
	if (CurrentStamina >= 2.0f)
	{
		CurrentStamina -= 2.0f;
		UE_LOG(LogTemp, Warning, TEXT("[SoccerPlayerCharacter] Heading ball"));
		// TODO: Implement header mechanics
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
