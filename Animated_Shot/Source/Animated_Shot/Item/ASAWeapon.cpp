// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/ASAWeapon.h"
#include "Components/StaticMeshComponent.h"

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
