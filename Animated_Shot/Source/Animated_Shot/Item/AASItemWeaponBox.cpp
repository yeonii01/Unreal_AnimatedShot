// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/AASItemWeaponBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/ASCollision.h"
#include "Interface/ASCharacterItemInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ASPlayerController.h"
#include "Item/ASItemData.h"
#include "CharacterStat/ASCharacterStatComponent.h"
#include "Item/ASWeaponItemData.h"

AAASItemWeaponBox::AAASItemWeaponBox()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AASAWeapon> WeaponRef(TEXT("/Game/Item/Weapon/Weapon_BP.Weapon_BP_C"));

	if (WeaponRef.Class)
	{
		WeaponClass = WeaponRef.Class;
	}
}

void AAASItemWeaponBox::BeginPlay()
{
	Super::BeginPlay();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AAASItemWeaponBox::OnOverlapBegin);
}

void AAASItemWeaponBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AASItemBox::Tick(DeltaTime);

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

void AAASItemWeaponBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	Effect->Activate(true);
	bIsOpening = true;
	SetActorEnableCollision(false);
	Effect->OnSystemFinished.AddDynamic(this, &AAASItemWeaponBox::OnEffectFinished);
	OverlapActor = OtherActor;
	OpenBox();
}

void AAASItemWeaponBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	//Destroy();
}

void AAASItemWeaponBox::OpenBox()
{
	FVector SpawnLocation1 = GetActorLocation() + FVector(60, 0, 80);
	FVector SpawnLocation2 = GetActorLocation() + FVector(-60, 0, 80);
	FRotator SpawnRotation = GetActorRotation() + FRotator(0, 90, -90);

	UStaticMesh* NewMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MyCharacter/weapon_02"));

	AASAWeapon* SpawnWeapon1 = GetWorld()->SpawnActor<AASAWeapon>(WeaponClass, SpawnLocation1, SpawnRotation);
	SpawnWeapon1->WeaponMesh->SetStaticMesh(NewMesh);
	AASAWeapon* SpawnWeapon2 = GetWorld()->SpawnActor<AASAWeapon>(WeaponClass, SpawnLocation2, SpawnRotation);
}

void AAASItemWeaponBox::SelectWeapon(int _key)
{
	UStaticMesh* NewMesh1 = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MyCharacter/weapon_01"));
	UStaticMesh* NewMesh2 = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MyCharacter/weapon_02"));

	if (_key == 1)
	{
		UASWeaponItemData* NewItem = NewObject<UASWeaponItemData>();
		NewItem->SetItemType(EItemType::Weapon1);
		NewItem->WeaponMesh = NewMesh1;
		Item = NewItem;
	}

	else if (_key == 2)
	{
		UASWeaponItemData* NewItem = NewObject<UASWeaponItemData>();
		NewItem->SetItemType(EItemType::Weapon2);

		NewItem->WeaponMesh = NewMesh2;
		Item = NewItem;
	}

	IASCharacterItemInterface* OverlappingPawn = Cast<IASCharacterItemInterface>(OverlapActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Item);
	}
}