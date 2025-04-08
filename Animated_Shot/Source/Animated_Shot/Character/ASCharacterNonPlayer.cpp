// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacterNonPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/AssetManager.h"
#include "AI/ASAIController.h"
#include "Components/DecalComponent.h"
#include "CharacterStat/ASCharacterStatComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ASComboActionData.h"
#include "../Item/ASItemBase.h"
#include "AI/ASAI.h"
#include "../Item/ASCoin.h"

AASCharacterNonPlayer::AASCharacterNonPlayer()
{
	GetMesh()->SetHiddenInGame(true);

	AIControllerClass = AASAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AnimClasses.SetNum(11);
	DeadMontages.SetNum(12);
	ComboActionMontages.SetNum(12);
	NPCMeshes.SetNum(12);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand0(TEXT("/Game/Monster/Animation/Beez/ABP_AS_Beez1.ABP_AS_Beez1_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand1(TEXT("/Game/Monster/Animation/DevilTree/ABP_AS_Deviltree1.ABP_AS_Deviltree1_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand2(TEXT("/Game/Monster/Desert/Animation/Anto/ABP_AS_Anto.ABP_AS_Anto"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand4(TEXT("/Game/Monster/Desert/Animation/Cactuso/ABP_AS_Cactuso.ABP_AS_Cactuso"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand3(TEXT("/Game/Monster/Animation/PlantaGeezer/ABP_AS_Planta1.ABP_AS_Planta1_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand5(TEXT("/Game/Monster/Animation/PlantaQueen/ABP_AS_Planta2.ABP_AS_Planta2"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand6(TEXT("/Game/Monster/Animation/PlantaShadow/ABP_AS_Planta3.ABP_AS_Planta3"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand7(TEXT("/Game/Monster/Desert/Animation/Wormo/ABP_AS_Worm.ABP_AS_Worm"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRefRand8(TEXT("/Game/Monster/Desert/Animation/Golem_Child/ABP_AS_GolemChild.ABP_AS_GolemChild"));

	AnimClasses[0] = AnimInstanceClassRefRand0.Class;
	AnimClasses[1] = AnimInstanceClassRefRand1.Class;
	AnimClasses[2] = AnimInstanceClassRefRand2.Class;
	AnimClasses[3] = AnimInstanceClassRefRand3.Class;
	AnimClasses[4] = AnimInstanceClassRefRand4.Class;
	AnimClasses[5] = AnimInstanceClassRefRand5.Class;
	AnimClasses[6] = AnimInstanceClassRefRand6.Class;
	AnimClasses[7] = AnimInstanceClassRefRand7.Class;
	AnimClasses[8] = AnimInstanceClassRefRand8.Class;


	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand0(TEXT("/Game/Monster/Animation/Beez/DeadMontage.DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand1(TEXT("/Game/Monster/Animation/DevilTree/DeadMontage.DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand2(TEXT("/Game/Monster/Desert/Animation/Anto/Anto_DeadMontage.Anto_DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand3(TEXT("/Game/Monster/Desert/Animation/Cactuso/CC_DeadMontage.CC_DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand4(TEXT("/Game/Monster/Animation/PlantaGeezer/DeadMontage.DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand5(TEXT("/Game/Monster/Animation/PlantaGeezer/DeadMontage.DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand6(TEXT("/Game/Monster/Animation/PlantaGeezer/DeadMontage.DeadMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand7(TEXT("/Game/Monster/Desert/Animation/Wormo/Wormo_DeadkMontage.Wormo_DeadkMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRefRand8(TEXT("/Game/Monster/Desert/Animation/Golem_Child/Child_DeadMontage.Child_DeadMontage"));


	DeadMontages[0] = DeadMontageRefRand0.Object;
	DeadMontages[1] = DeadMontageRefRand1.Object;
	DeadMontages[2] = DeadMontageRefRand2.Object;
	DeadMontages[3] = DeadMontageRefRand3.Object;
	DeadMontages[4] = DeadMontageRefRand4.Object;
	DeadMontages[5] = DeadMontageRefRand5.Object;
	DeadMontages[6] = DeadMontageRefRand6.Object;
	DeadMontages[7] = DeadMontageRefRand7.Object;
	DeadMontages[8] = DeadMontageRefRand8.Object;


	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRefRand0(TEXT("/Game/Monster/Animation/Beez/AM_AttackMontage.AM_AttackMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRefRand1(TEXT("/Game/Monster/Animation/DevilTree/AM_AttackMontage.AM_AttackMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRefRand2(TEXT("/Game/Monster/Desert/Animation/Anto/Anto_AttackMontage.Anto_AttackMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRefRand3(TEXT("/Game/Monster/Desert/Animation/Cactuso/CC_AttackMontage.CC_AttackMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRefRand4(TEXT("/Game/Monster/Animation/PlantaGeezer/AM_AttackMontage.AM_AttackMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRefRand5(TEXT("/Game/Monster/Animation/PlantaGeezer/AM_AttackMontage.AM_AttackMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRefRand6(TEXT("/Game/Monster/Animation/PlantaGeezer/AM_AttackMontage.AM_AttackMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRefRand7(TEXT("/Game/Monster/Desert/Animation/Wormo/Wormo_AttackMontage.Wormo_AttackMontage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRefRand8(TEXT("/Game/Monster/Desert/Animation/Golem_Child/Child_AttackMontage.Child_AttackMontage"));

	ComboActionMontages[0] = ComboActionMontageRefRand0.Object;
	ComboActionMontages[1] = ComboActionMontageRefRand1.Object;
	ComboActionMontages[2] = ComboActionMontageRefRand2.Object;
	ComboActionMontages[3] = ComboActionMontageRefRand3.Object;
	ComboActionMontages[4] = ComboActionMontageRefRand4.Object;
	ComboActionMontages[5] = ComboActionMontageRefRand5.Object;
	ComboActionMontages[6] = ComboActionMontageRefRand6.Object;
	ComboActionMontages[7] = ComboActionMontageRefRand7.Object;
	ComboActionMontages[8] = ComboActionMontageRefRand8.Object;

	//static ConstructorHelpers::FObjectFinder<UASComboActionData> ComboActionDataRef(TEXT("/Script/Animated_Shot.ASComboActionData'/Game/CharacterAction/ASA_ComboAttack1.ASA_ComboAttack1'"));
	//if (ComboActionDataRef.Object)
	//{
	//	ComboActionData = ComboActionDataRef.Object;
	//}

	// 기존 DecalComponent 제거 후 Static Mesh 사용
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MaskMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	// 원형 Masked 머터리얼 적용할 기본 Plane Mesh 설정
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane"));
	if (PlaneMesh.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(PlaneMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMeshRef0(TEXT("/Script/Engine.SkeletalMesh'/Game/Monster/Forest/Beez.Beez'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMeshRef1(TEXT("/Script/Engine.SkeletalMesh'/Game/Monster/Forest/Devil_Tree.Devil_Tree'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMeshRef2(TEXT("/Script/Engine.SkeletalMesh'/Game/Monster/Desert/Anto.Anto'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMeshRef3(TEXT("/Script/Engine.SkeletalMesh'/Game/Monster/Desert/Cactuso.Cactuso'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMeshRef4(TEXT("/Script/Engine.SkeletalMesh'/Game/Monster/Forest/Planta_Geezer.Planta_Geezer'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMeshRef5(TEXT("/Script/Engine.SkeletalMesh'/Game/Monster/Forest/Planta_Queen.Planta_Queen'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMeshRef6(TEXT("/Script/Engine.SkeletalMesh'/Game/Monster/Forest/Planta_Shadow.Planta_Shadow'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMeshRef7(TEXT("/Script/Engine.SkeletalMesh'/Game/Monster/Desert/Wormo.Wormo'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMeshRef8(TEXT("/Script/Engine.SkeletalMesh'/Game/Monster/Desert/Golem_Child.Golem_Child'"));
	NPCMeshes[0] = NPCMeshRef0.Object;
	NPCMeshes[1] = NPCMeshRef1.Object;
	NPCMeshes[2] = NPCMeshRef2.Object;
	NPCMeshes[3] = NPCMeshRef3.Object;
	NPCMeshes[4] = NPCMeshRef4.Object;
	NPCMeshes[5] = NPCMeshRef5.Object;
	NPCMeshes[6] = NPCMeshRef6.Object;
	NPCMeshes[7] = NPCMeshRef7.Object;
	NPCMeshes[8] = NPCMeshRef8.Object;
	//// 머터리얼 로드 및 적용
	//static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaskedMaterial(TEXT("/Script/Engine.Material'/Game/Monster/M_AttackRange.M_AttackRange'"));
	//if (MaskedMaterial.Succeeded())
	//{
	//	MaskedMaterialInstance = UMaterialInstanceDynamic::Create(MaskedMaterial.Object, this);
	//	StaticMeshComponent->SetMaterial(0, MaskedMaterialInstance);
	//}


	// 크기 및 회전 설정
	StaticMeshComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 1.0f));  // Plane 크기 조절
	StaticMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));  // 바닥을 향하도록 회전
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -95.0f));  // 바닥을 향하도록 회전
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AASCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//ensure(NPCMeshes.Num() > 0);
	//int32 RandIndex = 2/*FMath::RandRange(0, 7)*/;
	//NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &AASCharacterNonPlayer::NPCMeshLoadCompleted));
	//GetMesh()->SetAnimInstanceClass(AnimClasses[RandIndex]);
	//DeadMontage = DeadMontages[RandIndex];
	//ComboActionMontage = ComboActionMontages[RandIndex];
	//if (SelectedNPCMesh.IsValid())
	//{
	//	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
	//		SelectedNPCMesh.ToSoftObjectPath(),
	//		FStreamableDelegate::CreateUObject(this, &AASCharacterNonPlayer::NPCMeshLoadCompleted)
	//	);
	//
	if (NPCMeshes[8])
	{
		GetMesh()->SetSkeletalMesh(NPCMeshes[8]);
		GetMesh()->SetVisibility(true);
	}
	if (AnimClasses[8])
	{
		GetMesh()->SetAnimInstanceClass(AnimClasses[8]);
	}

	if (DeadMontages[8])
	{
		DeadMontage = DeadMontages[8];
	}

	if (ComboActionMontages[8])
	{
		ComboActionMontage = ComboActionMontages[8];
	}

	UMaterialInterface* MaskedMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Monster/M_AttackRange.M_AttackRange"));
	if (MaskedMaterial)
	{
		MaskedMaterialInstance = UMaterialInstanceDynamic::Create(MaskedMaterial, this);
		StaticMeshComponent->SetMaterial(0, MaskedMaterialInstance);
	}
	StaticMeshComponent->SetRelativeScale3D(FVector(GetAIAttackRange() / 50.f, GetAIAttackRange() / 50.f, 1.f));

	//SetLevel(2);
}

void AASCharacterNonPlayer::SetNPC(int index)
{
	if (NPCMeshes[index])
	{
		GetMesh()->SetSkeletalMesh(NPCMeshes[index]);
		GetMesh()->SetVisibility(true);
	}
	if (AnimClasses[index])
	{
		GetMesh()->SetAnimInstanceClass(AnimClasses[index]);
	}

	if (DeadMontages[index])
	{
		DeadMontage = DeadMontages[index];
	}

	if (ComboActionMontages[index])
	{
		ComboActionMontage = ComboActionMontages[index];
	}
}

void AASCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	AASAIController* ASAIController = Cast<AASAIController>(GetController());
	if (ASAIController)
	{
		ASAIController->StopAI();
	}

	DropItem();
	DropCoin();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([&](){
		Destroy();
		}), DeadEventDelayTime, false);

}

void AASCharacterNonPlayer::NPCMeshLoadCompleted()
{
	/*if (NPCMeshHandle.IsValid())
	{
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());
		if (NPCMesh)
		{
			GetMesh()->SetSkeletalMesh(NPCMesh);
			GetMesh()->SetHiddenInGame(false);
		}
	}

	NPCMeshHandle->ReleaseHandle();*/
}

void AASCharacterNonPlayer::DropItem()
{
	if (DropItems.IsEmpty())
		return;

	int32 DropCount = FMath::RandRange(0, MaxDropItemNum);
	TArray<TSubclassOf<AASItemBase>> AvailableItems = DropItems; // 원본 유지

	for (int32 i = 0; i < DropCount; ++i)
	{
		if (AvailableItems.Num() == 0) // 남은 아이템이 없으면 중단
			break;

		int32 DropItemIndex = FMath::RandRange(0, AvailableItems.Num() - 1);
		TSubclassOf<AASItemBase> DropItem = AvailableItems[DropItemIndex];

		if (!IsValid(DropItem))
			continue;

		FVector RandomDirection = FMath::VRand();
		RandomDirection.Z = 0.f;
		RandomDirection *= FMath::RandRange(MinDropRange, MaxDropRange);
		FVector SpawnLocation = GetActorLocation() + RandomDirection;

		GetWorld()->SpawnActor<AASItemBase>(DropItem, SpawnLocation, GetActorRotation());

		AvailableItems.RemoveAt(DropItemIndex); // 중복 제거
	}


	/*if (DropItems.IsEmpty())
		return;

	int32 DropCount = FMath::RandRange(0, MaxDropItemNum);

	for (int32 i = 0; i < DropCount; ++i)
	{
		int32 DropItemIndex = FMath::RandRange(0, DropItems.Num() - 1);

		TSubclassOf<AASItemBase> DropItem = DropItems[DropItemIndex];

		if (!IsValid(DropItem))
			continue;

		FVector RandomDirection = FMath::VRand();
		RandomDirection.Z = 0.f;
		RandomDirection *= FMath::RandRange(MinDropRange, MaxDropRange);

		FVector SpawnLocation = GetActorLocation() + RandomDirection;

		GetWorld()->SpawnActor<AASItemBase>(DropItem, SpawnLocation, GetActorRotation());
	}*/
}

void AASCharacterNonPlayer::DropCoin()
{
	if (!IsValid(CoinClass))
		return;

	FVector RandomDirection = FMath::VRand();
	RandomDirection.Z = 0.f;
	RandomDirection *= FMath::RandRange(MinDropRange, MaxDropRange);

	FVector SpawnLocation = GetActorLocation() + RandomDirection;

	AASCoin* Coin = GetWorld()->SpawnActor<AASCoin>(CoinClass, SpawnLocation, GetActorRotation());
	if(IsValid(Coin))
		Coin->SetCoin(FMath::RandRange(MinCoinValue, MaxCoinValue));
}

void AASCharacterNonPlayer::SetCircleColor(FLinearColor NewColor)
{
	if (MaskedMaterialInstance) // DecalMaterialInstance → MaskedMaterialInstance
	{
		MaskedMaterialInstance->SetVectorParameterValue(FName("CircleColor"), NewColor);
	}
}

float AASCharacterNonPlayer::GetAIPatrolRadius()
{
	return 700.f;
}

float AASCharacterNonPlayer::GetAIDetectRange()
{
	return 400.f;
}

float AASCharacterNonPlayer::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}

float AASCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.f;
}

void AASCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AASCharacterNonPlayer::AttackByAI()
{
	ProcessComboCommand();
}

void AASCharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();
}

void AASCharacterNonPlayer::SetTargetPos(const Protocol::PosInfo& pos)
{
	AAIController* AICon = Cast<AAIController>(GetController());
	if (AICon == nullptr)
		return;

	UBlackboardComponent* BB = AICon->GetBlackboardComponent();
	if (BB == nullptr)
		return;
	FVector PatrolPos;
	FVector Pos = FVector(pos.x(), pos.y(), pos.z());
	if (!Pos.IsNearlyZero())
	{
		PatrolPos = FVector(pos.x(), pos.y(), pos.z());
	}
	else
	{
		float RandX = FMath::FRandRange(-500.f, 500.f);
		float RandY = FMath::FRandRange(-500.f, 500.f);
		PatrolPos = FVector(GetActorLocation().X + RandX, GetActorLocation().Y + RandY, GetActorLocation().Z);

		UE_LOG(LogTemp, Warning, TEXT("[Fallback] No valid PosInfo! Generated random patrol pos: (%.2f, %.2f, %.2f)"),
			PatrolPos.X, PatrolPos.Y, PatrolPos.Z);
	}
	BB->SetValueAsVector(BBKEY_PATROLPOS, PatrolPos);
}
