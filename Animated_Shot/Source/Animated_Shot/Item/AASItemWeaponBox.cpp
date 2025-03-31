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
#include "Animated_Shot.h"
#include "Network/ASGameInstance.h"
#include "Character/ASCharacterPlayer.h"

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

	if (bIsOpening)
	{
		AASPlayerController* PlayerController = Cast<AASPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PlayerController && PlayerController->IsInputKeyDown(EKeys::Q))
		{
			SelectWeapon(0, OverlapActor);
			Protocol::C_PARTY_WEAPON WeaponPkt;
			{
				WeaponPkt.set_weapon(false);
			}
			SEND_PACKET(WeaponPkt);
		}
		if (PlayerController && PlayerController->IsInputKeyDown(EKeys::E))
		{
			SelectWeapon(1, OverlapActor);
			Protocol::C_PARTY_WEAPON WeaponPkt;
			{
				WeaponPkt.set_weapon(true);
			}
			SEND_PACKET(WeaponPkt);
		}
	}
}

void AAASItemWeaponBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	AASCharacterBase* Player = Cast<AASCharacterBase>(OtherActor);
	if (!Player->IsMyPlayer())
		return;
	if (!bIsOpening)
	{
		Effect->Activate(true);
		bIsOpening = true;
		SetActorEnableCollision(false);
		Effect->OnSystemFinished.AddDynamic(this, &AAASItemWeaponBox::OnEffectFinished);
		OverlapActor = OtherActor;
		OpenBox();
	}
}

void AAASItemWeaponBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	//Destroy();
}

void AAASItemWeaponBox::OpenBox()
{
	if (!SpawnWeapons.IsEmpty())
		return;

	FVector SpawnLocation1 = GetActorLocation() + FVector(60, 0, 80);
	FVector SpawnLocation2 = GetActorLocation() + FVector(-60, 0, 80);
	FRotator SpawnRotation = GetActorRotation() + FRotator(0, 90, -90);

	UStaticMesh* NewMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MyCharacter/weapon_02"));
	
	SpawnWeapons.Reset(2);

	SpawnWeapons.Add(GetWorld()->SpawnActor<AASAWeapon>(WeaponClass, SpawnLocation1, SpawnRotation));
	SpawnWeapons[0]->WeaponMesh->SetStaticMesh(NewMesh);
	SpawnWeapons[0]->SetWeaponType(EItemType::Weapon1);
	SpawnWeapons.Add(GetWorld()->SpawnActor<AASAWeapon>(WeaponClass, SpawnLocation2, SpawnRotation));
	SpawnWeapons[1]->SetWeaponType(EItemType::Weapon2);

	for (auto& iter : SpawnWeapons)
	{
		if (IsValid(iter))
			iter->IsInBox = true;
	}
}

void AAASItemWeaponBox::SelectWeapon(int _key, AActor* Party)
{
	if (SpawnWeapons.IsEmpty())
		return;

	if (!IsValid(SpawnWeapons[_key]))
		return;

	SpawnWeapons[_key]->IsInBox = false;
	if(Party == nullptr)
		SpawnWeapons[_key]->PickUp(OverlapActor);
	else
		SpawnWeapons[_key]->PickUp(Party);


	for (auto& iter : SpawnWeapons)
	{
		if(IsValid(iter))
			iter->Destroy();
	}
	SpawnWeapons.Reset(2);
}