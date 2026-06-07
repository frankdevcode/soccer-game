// Copyright Frank Dev Code. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoccerCharacterRig.generated.h"

class ASoccerPlayerCharacter;
class USkeletalMesh;
class UAnimInstance;

/**
 * @class USoccerCharacterRig
 * @brief Defines a player character rig and appearance configuration.
 */
UCLASS(BlueprintType)
class SOCCERGAME_API USoccerCharacterRig : public UObject
{
	GENERATED_BODY()

public:
	USoccerCharacterRig();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Rig")
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Rig")
	TSubclassOf<UAnimInstance> AnimationBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Rig")
	float HeightScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Rig")
	float WeightScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soccer|Rig")
	TMap<FName, FTransform> BoneOverrides;

	UFUNCTION(BlueprintCallable, Category = "Soccer|Rig")
	void ApplyToCharacter(ASoccerPlayerCharacter* Character) const;
};
