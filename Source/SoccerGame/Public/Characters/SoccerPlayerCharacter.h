// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SoccerPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UCharacterMovementComponent;
class UAnimInstance;
class USkeletalMesh;
class ASoccerBall;

/**
 * @enum EPlayerPosition
 * @brief Player position on field
 */
UENUM(BlueprintType)
enum class EPlayerPosition : uint8
{
	Goalkeeper UMETA(DisplayName = "Goalkeeper"),
	Defender UMETA(DisplayName = "Defender"),
	Midfielder UMETA(DisplayName = "Midfielder"),
	Forward UMETA(DisplayName = "Forward")
};

/**
 * @class ASoccerPlayerCharacter
 * @brief Player character for Soccer-Game
 * 
 * Handles player movement, input, animation, and ball interaction.
 * Supports both AI and player-controlled instances.
 */
UCLASS(Blueprintable, BlueprintType)
class SOCCERGAME_API ASoccerPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASoccerPlayerCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Player initialization
	UFUNCTION(BlueprintCallable, Category = "Soccer|Player")
	void InitializePlayer(int32 InTeamId, EPlayerPosition InPosition, int32 InPlayerNumber);

	// Character appearance and rigging
	UFUNCTION(BlueprintCallable, Category = "Soccer|Character")
	void ConfigurePlayerModel(USkeletalMesh* Mesh, TSubclassOf<UAnimInstance> AnimBP);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Character")
	void ApplyCharacterRig();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Character")
	USoccerCharacterRig* CharacterRigDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Character")
	USkeletalMesh* DefaultSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Character")
	TSubclassOf<UAnimInstance> DefaultAnimInstanceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Character")
	float CharacterHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Character")
	float CharacterWeight;

	// Movement
	UFUNCTION(BlueprintCallable, Category = "Soccer|Movement")
	void Move(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Movement")
	void MoveForward(float Value);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Movement")
	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Movement")
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Movement")
	void ZoomCamera(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Movement")
	void Sprint();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Movement")
	void StopSprinting();

	// Ball interaction
	UFUNCTION(BlueprintCallable, Category = "Soccer|Ball")
	void KickBall(float Power = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Ball")
	void PassBall(AActor* TargetPlayer, float Power = 0.7f);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Ball")
	void HeadBall();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Ball")
	void JumpForBall();

	// Goalkeeper mechanics
	UFUNCTION(BlueprintCallable, Category = "Soccer|Goalkeeper")
	void DiveForSave();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Goalkeeper")
	void AttemptGoalkeeperSave();

	UFUNCTION(BlueprintCallable, Category = "Soccer|Goalkeeper")
	bool IsInGoalkeeperZone() const;

	// Getters
	UFUNCTION(BlueprintCallable, Category = "Soccer|Player")
	int32 GetTeamId() const { return TeamId; }

	UFUNCTION(BlueprintCallable, Category = "Soccer|Player")
	EPlayerPosition GetPosition() const { return PlayerPosition; }

	UFUNCTION(BlueprintCallable, Category = "Soccer|Player")
	int32 GetPlayerNumber() const { return PlayerNumber; }

	UFUNCTION(BlueprintCallable, Category = "Soccer|Player")
	float GetStamina() const { return CurrentStamina; }

	UFUNCTION(BlueprintCallable, Category = "Soccer|Player")
	float GetMaxStamina() const { return MaxStamina; }

protected:
	// Player information
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Player")
	int32 TeamId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Player")
	EPlayerPosition PlayerPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Player")
	int32 PlayerNumber;

	// Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Movement")
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Movement")
	float SprintSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Movement")
	float Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Movement")
	float Deceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Movement")
	bool bIsSprinting;

	// Ball interaction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Ball")
	float BallInteractionRange;

	// Goalkeeper settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Goalkeeper")
	float GoalkeeperSaveRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Goalkeeper")
	float GoalkeeperDiveForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Goalkeeper")
	float GoalkeeperDiveCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Goalkeeper")
	float GoalkeeperDiveStaminaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Goalkeeper")
	float GoalkeeperPushbackStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Goalkeeper")
	bool bGoalkeeperCanDive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Goalkeeper")
	float LastGoalkeeperDiveTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Goalkeeper")
	float GoalkeeperSaveAnimationDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Goalkeeper")
	UInputAction* SaveAction;

	// Stamina system
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Stamina")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Stamina")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Stamina")
	float StaminaRegenerationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Stamina")
	float SprintStaminaCost;

	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Camera")
	float CameraFollowDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Camera")
	float MinCameraDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Camera")
	float MaxCameraDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Camera")
	float CameraZoomStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Camera")
	float CameraPitchMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Camera")
	float CameraPitchMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Camera")
	float CameraLagSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Camera")
	float CameraRotationLagSpeed;

	// Enhanced Input System
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|Input")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|Input")
	class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|Input")
	class UInputAction* KickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|Input")
	class UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Soccer|Input")
	class UInputAction* JumpAction;

private:
	// Internal stamina management
	void UpdateStamina(float DeltaTime);
	void RegenerateStamina(float DeltaTime);
	void ConsumeSprint(float DeltaTime);

	// Character setup
	void SetupCharacterAppearance();

	// Goalkeeper state update
	void UpdateGoalkeeperState(float DeltaTime);

	// Ball interaction helpers
	ASoccerBall* FindNearestBall(float MaxDistance) const;

	// Animation
	void UpdateAnimations();

	FVector LastMovementInput;
	FVector2D MovementInput;
	float DesiredRotationSpeed;
};
