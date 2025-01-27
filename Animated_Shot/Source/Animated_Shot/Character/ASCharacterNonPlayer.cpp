// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacterNonPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

AASCharacterNonPlayer::AASCharacterNonPlayer()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>NonPlayerMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Monster/Forest/Flower_Dryad.Flower_Dryad'"));
	if (NonPlayerMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(NonPlayerMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Monster/Animation/FlowerDyad/ABP_AS_FlowerDyad.ABP_AS_FlowerDyad_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Monster/Animation/FlowerDyad/DeadMontage.DeadMontage'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}
}

void AASCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([&](){
		Destroy();
		}), DeadEventDelayTime, false);
}