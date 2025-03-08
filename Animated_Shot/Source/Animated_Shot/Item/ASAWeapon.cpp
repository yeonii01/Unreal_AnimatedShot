// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/ASAWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "ASWeaponItemData.h"
#include "../Character/ASCharacterPlayer.h"

// Sets default values
AASAWeapon::AASAWeapon()
{
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/MyCharacter/weapon_01.weapon_01'"));
    if (MeshAsset.Succeeded())
    {
        WeaponMesh->SetStaticMesh(MeshAsset.Object);
    }

    ItemData = NewObject<UASWeaponItemData>();

    SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AASAWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AASAWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FRotator NewRotation = FRotator(-50.f * DeltaTime, 0.f, 0.f);
    AddActorLocalRotation(NewRotation);
}

void AASAWeapon::SetWeaponType(EItemType Type)
{
    UASWeaponItemData* WeaponData = Cast<UASWeaponItemData>(ItemData);

    if (!WeaponData)
        return;

    UStaticMesh* NewMesh1 = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MyCharacter/weapon_01"));
    UStaticMesh* NewMesh2 = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MyCharacter/weapon_02"));

    ItemData->SetItemType(Type);

    switch (Type)
    {
    case EItemType::Weapon1:
        WeaponData->WeaponMesh = NewMesh1;
        break;
    case EItemType::Weapon2:
        WeaponData->WeaponMesh = NewMesh2;
        break;
    default:
        break;
    }
}

void AASAWeapon::PickUp(AActor* Actor)
{
    if (IsInBox || !IsValid(Actor))
        return;

    AASCharacterPlayer* Player = Cast<AASCharacterPlayer>(Actor);

    if (!IsValid(Player))
        return;

    if (Player->IsEquipWeapon())
        Super::PickUp(Actor);
    else
    {
        IASCharacterItemInterface* OverlappingPawn = Cast<IASCharacterItemInterface>(Actor);
        if (OverlappingPawn)
            OverlappingPawn->TakeItem(ItemData);
    }
}
