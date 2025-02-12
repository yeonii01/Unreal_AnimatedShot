// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacterNonPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/AssetManager.h"

AASCharacterNonPlayer::AASCharacterNonPlayer()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>NonPlayerMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Monster/Forest/Flower_Dryad.Flower_Dryad'"));
	if (NonPlayerMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(NonPlayerMeshRef.Object);
	}
	GetMesh()->SetHiddenInGame(true);
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

void AASCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//ensure(NPCMeshes.Num() > 0);
	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &AASCharacterNonPlayer::NPCMeshLoadCompleted));
}

void AASCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([&](){
		Destroy();
		}), DeadEventDelayTime, false);
}

void AASCharacterNonPlayer::NPCMeshLoadCompleted()
{
	if (NPCMeshHandle.IsValid())
	{
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());
		if (NPCMesh)
		{
			GetMesh()->SetSkeletalMesh(NPCMesh);
			GetMesh()->SetHiddenInGame(false);
		}
	}

	NPCMeshHandle->ReleaseHandle();
}
