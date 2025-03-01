// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ASPotion.h"

// Sets default values
AASPotion::AASPotion()
{
    PotionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PotionMesh"));
    RootComponent = PotionMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/InventorySystem/Items/material/1.1'"));
    if (MeshAsset.Succeeded())
    {
        PotionMesh->SetStaticMesh(MeshAsset.Object);
    }

    SetActorScale3D(FVector(0.5, 0.5f, 0.5f));
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AASPotion::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AASPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FRotator NewRotation = FRotator(-50.f * DeltaTime, 0.f, 0.f);
    AddActorLocalRotation(NewRotation);
}

