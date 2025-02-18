// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacterNonPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/AssetManager.h"
#include "AI/ASAIController.h"

AASCharacterNonPlayer::AASCharacterNonPlayer()
{
	GetMesh()->SetHiddenInGame(true);

	AIControllerClass = AASAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AnimClasses.SetNum(12);
	DeadMontages.SetNum(12);
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand0(TEXT("/Game/Monster/Animation/Beez/ABP_AS_Beez.ABP_AS_Beez_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand1(TEXT("/Game/Monster/Animation/DevilTree/ABP_AS_Deviltree.ABP_AS_Deviltree_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand2(TEXT("/Game/Monster/Animation/FlowerDyad/ABP_AS_FlowerDyad.ABP_AS_FlowerDyad_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand3(TEXT("/Game/Monster/Animation/PlantaGeezer/ABP_AS_Planta.ABP_AS_Planta_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand4(TEXT("/Game/Monster/Animation/PlantaKid/ABP_AS_PlantaKid.ABP_AS_PlantaKid_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand5(TEXT("/Game/Monster/Animation/PlantaQueen/ABP_AS_PlantaQueen.ABP_AS_PlantaQueen_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand6(TEXT("/Game/Monster/Animation/PlantaShadow/ABP_AS_PlantaShadow.ABP_AS_PlantaShadow_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand7(TEXT("/Game/Monster/Animation/PlantaSlave/ABP_AS_PlantaSlave.ABP_AS_PlantaSlave_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand8(TEXT("/Game/Monster/Animation/Rabby/ABP_AS_Rabby.ABP_AS_Rabby_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand9(TEXT("/Game/Monster/Animation/RabbyQueen/ABP_AS_RabbyQueen.ABP_AS_RabbyQueen_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand10(TEXT("/Game/Monster/Animation/RabbyYoung/ABP_AS_RabbyYoung.ABP_AS_RabbyYoung_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand11(TEXT("/Game/Monster/Animation/Turnipa/ABP_AS_Turnipa.ABP_AS_Turnipa_C"));

	AnimClasses[0] = AnimInstanceClassRefRand0.Class;
	AnimClasses[1] = AnimInstanceClassRefRand1.Class;
	AnimClasses[2] = AnimInstanceClassRefRand2.Class;
	AnimClasses[3] = AnimInstanceClassRefRand3.Class;
	AnimClasses[4] = AnimInstanceClassRefRand4.Class;
	AnimClasses[5] = AnimInstanceClassRefRand5.Class;
	AnimClasses[6] = AnimInstanceClassRefRand6.Class;
	AnimClasses[7] = AnimInstanceClassRefRand7.Class;
	AnimClasses[8] = AnimInstanceClassRefRand8.Class;
	AnimClasses[9] = AnimInstanceClassRefRand9.Class;
	AnimClasses[10] = AnimInstanceClassRefRand10.Class;
	AnimClasses[11] = AnimInstanceClassRefRand11.Class;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand0(TEXT("/Game/Monster/Animation/Beez/DeadMontage.DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand1(TEXT("/Game/Monster/Animation/DevilTree/DeadMontage.DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand2(TEXT("/Game/Monster/Animation/FlowerDyad/DeadMontage.DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand3(TEXT("/Game/Monster/Animation/PlantaGeezer/DeadMontage.DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand4(TEXT("/Game/Monster/Animation/PlantaKid/DeadMontage.DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand5(TEXT("/Game/Monster/Animation/PlantaQueen/DeadMontage.DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand6(TEXT("/Game/Monster/Animation/PlantaShadow/DeadMontage.DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand7(TEXT("/Game/Monster/Animation/PlantaSlave/DeadMontage.DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand8(TEXT("/Game/Monster/Animation/Rabby/DeadMontage.DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand9(TEXT("/Game/Monster/Animation/RabbyQueen/DeadMontage.DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand10(TEXT("/Game/Monster/Animation/RabbyYoung/DeadMontage.DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand11(TEXT("/Game/Monster/Animation/Turnipa/DeadMontage.DeadMontage"));

	DeadMontages[0] = DeadMontageRefRand0.Object;
	DeadMontages[1] = DeadMontageRefRand1.Object;
	DeadMontages[2] = DeadMontageRefRand2.Object;
	DeadMontages[3] = DeadMontageRefRand3.Object;
	DeadMontages[4] = DeadMontageRefRand4.Object;
	DeadMontages[5] = DeadMontageRefRand5.Object;
	DeadMontages[6] = DeadMontageRefRand6.Object;
	DeadMontages[7] = DeadMontageRefRand7.Object;
	DeadMontages[8] = DeadMontageRefRand8.Object;
	DeadMontages[9] = DeadMontageRefRand9.Object;
	DeadMontages[10] =DeadMontageRefRand10.Object;
	DeadMontages[11] =DeadMontageRefRand11.Object;
}

void AASCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ensure(NPCMeshes.Num() > 0);
	int32 RandIndex = FMath::RandRange(0, 7);
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &AASCharacterNonPlayer::NPCMeshLoadCompleted));
	GetMesh()->SetAnimInstanceClass(AnimClasses[RandIndex]);
	DeadMontage = DeadMontages[RandIndex];
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