// Copyright Frank Dev Code. All Rights Reserved.

#include "Characters/SoccerCharacterRig.h"
#include "Characters/SoccerPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

USoccerCharacterRig::USoccerCharacterRig()
	: SkeletalMesh(nullptr)
	, AnimationBlueprint(nullptr)
	, HeightScale(1.0f)
	, WeightScale(1.0f)
{
}

void USoccerCharacterRig::ApplyToCharacter(ASoccerPlayerCharacter* Character) const
{
	if (!Character)
	{
		return;
	}

	USkeletalMeshComponent* MeshComponent = Character->GetMesh();
	if (!MeshComponent)
	{
		return;
	}

	if (SkeletalMesh)
	{
		MeshComponent->SetSkeletalMesh(SkeletalMesh);
	}

	if (AnimationBlueprint)
	{
		MeshComponent->SetAnimInstanceClass(AnimationBlueprint);
	}

	const FVector MeshScale(HeightScale, WeightScale, HeightScale);
	MeshComponent->SetRelativeScale3D(MeshScale);

	if (UCapsuleComponent* Capsule = Character->GetCapsuleComponent())
	{
		const float BaseHalfHeight = 88.0f;
		const float BaseRadius = 34.0f;

		Capsule->SetCapsuleHalfHeight(BaseHalfHeight * HeightScale);
		Capsule->SetCapsuleRadius(BaseRadius * FMath::Max(0.8f, WeightScale));
	}

	for (const TPair<FName, FTransform>& BoneOverride : BoneOverrides)
	{
		if (MeshComponent->DoesSocketExist(BoneOverride.Key))
		{
			MeshComponent->SetBoneTransformByName(BoneOverride.Key, BoneOverride.Value, EBoneSpaces::ComponentSpace);
		}
	}
}
