// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoccerBall.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UBallPhysicsComponent;
class USoccerAudioComponent;

/**
 * @class ASoccerBall
 * @brief Ball actor with physical simulation for passes, kicks, and headers.
 */
UCLASS()
class SOCCERGAME_API ASoccerBall : public AActor
{
	GENERATED_BODY()

public:
	ASoccerBall();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Soccer|Ball")
	void ApplyKick(const FVector& Direction, float Power = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Ball")
	void ApplyPass(const FVector& Direction, float Power = 0.7f);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Ball")
	void ApplyHeader(const FVector& Direction, float Power = 0.6f);

	UFUNCTION(BlueprintCallable, Category = "Soccer|Ball")
	void ResetBall(const FVector& Location);

protected:
	void HandleImpact(const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Ball")
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Ball")
	UStaticMeshComponent* BallMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Ball")
	UBallPhysicsComponent* BallPhysicsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Soccer|Audio")
	USoccerAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Ball")
	float KickForceMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Ball")
	float PassForceMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Ball")
	float HeaderForceMultiplier;
};
