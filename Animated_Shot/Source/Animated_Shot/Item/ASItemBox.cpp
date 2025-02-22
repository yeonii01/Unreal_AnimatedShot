// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ASItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/ASCollision.h"
#include "Interface/ASCharacterItemInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ASPlayerController.h"
#include "Item/ASItemData.h"
#include "Item/ASWeaponItemData.h"
#include "Engine/AssetManager.h"

// Sets default values
AASItemBox::AASItemBox()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_ASTRIGGER);
	Trigger->SetBoxExtent(FVector(60.f, 42.f, 30.f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AASItemBox::OnOverlapBegin);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/CemeteryDungeon/Meshes/Chests/SM_cdmediumchest1top.SM_cdmediumchest1top'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.f, -80.f, -30.f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	// 두 번째 메쉬 추가
	BoxBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxBodyMesh"));
	BoxBodyMesh->SetupAttachment(Trigger); // 기존 Mesh에 부착
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxBodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/CemeteryDungeon/Meshes/Chests/SM_cdmediumchest1bot.SM_cdmediumchest1bot'"));
	if (BoxBodyMeshRef.Succeeded())
	{
		BoxBodyMesh->SetStaticMesh(BoxBodyMeshRef.Object);
	}
	BoxBodyMesh->SetRelativeLocation(FVector(0.f, -85.f, -65.f)); // 두 번째 메쉬의 상대 위치
	BoxBodyMesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object);
		Effect->bAutoActivate = false;
	}
	Effect->SetRelativeLocation(FVector(0.f, 0.f, 5.f));
	Effect->SetTranslucentSortPriority(1);

	PrimaryActorTick.bCanEverTick = true;
}

void AASItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsOpening && CurrentRotation > -90.f)
	{
		float RotationSpeed = 90.f;
		float RotationDelta = RotationSpeed * DeltaTime;
		CurrentRotation = FMath::Clamp(CurrentRotation - RotationDelta, -90.f, 0.f);
		Mesh->SetRelativeRotation(FRotator(0.f, 0.f, CurrentRotation));
	}
}

void AASItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("ASItemData"), Assets);
	ensure(0 < Assets.Num());

	int32 RandomIndex = FMath::RandRange(2, Assets.Num() - 1);
	FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}
	Item = Cast<UASItemData>(AssetPtr.Get());
	ensure(Item);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AASItemBox::OnOverlapBegin);
}

void AASItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	IASCharacterItemInterface* OverlappingPawn = Cast<IASCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Item);
	}

	Effect->Activate(true);
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	Effect->OnSystemFinished.AddDynamic(this, &AASItemBox::OnEffectFinished);
}

void AASItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	//Destroy();
}