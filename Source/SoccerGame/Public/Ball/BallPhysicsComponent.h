// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BallPhysicsComponent.generated.h"

/**
 * @class UBallPhysicsComponent
 * @brief Simulates ball physics state and velocity over time.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SOCCERGAME_API UBallPhysicsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBallPhysicsComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Soccer|Ball")
	void SimulatePhysics(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Ball")
	void ApplyImpulse(const FVector& Impulse);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Ball")
	void SetVelocity(const FVector& NewVelocity);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Ball")
	const FVector& GetVelocity() const { return Velocity; }

	UFUNCTION(BlueprintCallable, Category = "Soccer|Ball")
	void SetSpin(const FVector& NewSpin);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Ball")
	const FVector& GetSpin() const { return Spin; }

	// Ball configuration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Ball")
	float Mass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Ball")
	float AirResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Ball")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Ball")
	float MagnusCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Ball")
	float GroundFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Ball")
	float SpinDamping;

private:
	FVector Velocity;
	FVector Spin;
};
