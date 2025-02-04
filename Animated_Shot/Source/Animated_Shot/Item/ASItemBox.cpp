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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object);
		Effect->bAutoActivate = false;
	}
	Effect->SetRelativeLocation(FVector(0.f, 0.f, 5.f));
	Effect->SetTranslucentSortPriority(1);
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<AASAWeapon> WeaponRef(TEXT("/Script/CoreUObject.Class'/Script/Animated_Shot.ASAWeapon_C'"));
	if (WeaponRef.Class)
	{
		WeaponClass = WeaponRef.Class;
	}
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
	if (bIsOpening)
	{
		AASPlayerController* PlayerController = Cast<AASPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PlayerController && PlayerController->IsInputKeyDown(EKeys::Q))
		{
			SelectWeapon(1);
		}
		if (PlayerController && PlayerController->IsInputKeyDown(EKeys::E))
		{
			SelectWeapon(2);
		}
	}
}

void AASItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	Effect->Activate(true);
	bIsOpening = true;
	SetActorEnableCollision(false);
	Effect->OnSystemFinished.AddDynamic(this, &AASItemBox::OnEffectFinished);
	OverlapActor = OtherActor;
	OpenBox();
}

void AASItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	//Destroy();
}

void AASItemBox::OpenBox()
{
	FVector SpawnLocation1 = GetActorLocation() + FVector(60, 0, 80);
	FVector SpawnLocation2 = GetActorLocation() + FVector(-60, 0, 80);
	FRotator SpawnRotation = GetActorRotation() + FRotator(0, 90, -90);

	UStaticMesh* NewMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MyCharacter/weapon_02.weapon_02"));

	AASAWeapon* SpawnWeapon1 = GetWorld()->SpawnActor<AASAWeapon>(WeaponClass, SpawnLocation1, SpawnRotation);
	SpawnWeapon1->WeaponMesh->SetStaticMesh(NewMesh);
	AASAWeapon* SpawnWeapon2 = GetWorld()->SpawnActor<AASAWeapon>(WeaponClass, SpawnLocation2, SpawnRotation);
}

void AASItemBox::SelectWeapon(int _key)
{
	UStaticMesh* NewMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MyCharacter/weapon_02.weapon_02"));

	if (_key == 2)
	{
		UASWeaponItemData* NewItem = NewObject<UASWeaponItemData>();
		NewItem->SetItemType(EItemType::Weapon2);
		NewItem->WeaponMesh = NewMesh;
		Item = NewItem;
	}

	IASCharacterItemInterface* OverlappingPawn = Cast<IASCharacterItemInterface>(OverlapActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Item);
	}
}
