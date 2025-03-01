// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ASPotionItemBox.h"
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

AASPotionItemBox::AASPotionItemBox()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AASPotion> PotionRef(TEXT("/Game/InventorySystem/Items/item/PotionBP.PotionBP_C"));

	if (PotionRef.Class)
	{
		PotionClass = PotionRef.Class;
	}
}

void AASPotionItemBox::BeginPlay()
{
	Super::BeginPlay();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AASPotionItemBox::OnOverlapBegin);
}

void AASPotionItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AASPotionItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (!bIsOpening)
	{
		Effect->Activate(true);
		bIsOpening = true;
		SetActorEnableCollision(false);
		Effect->OnSystemFinished.AddDynamic(this, &AASPotionItemBox::OnEffectFinished);
		OverlapActor = OtherActor;
	}
}

void AASPotionItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	OpenBox();
}

void AASPotionItemBox::OpenBox()
{
	FVector SpawnLocation1 = GetActorLocation() + FVector(0, 0, 80);

	FRotator SpawnRotation = GetActorRotation() + FRotator(0, 90, -90);

	AASPotion* SpawnPotion = GetWorld()->SpawnActor<AASPotion>(PotionClass, SpawnLocation1, SpawnRotation);
}
