// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ASScroll.h"

AASScroll::AASScroll()
{
    ScrollMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScrollMesh"));
    RootComponent = ScrollMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Item/Scroll/_simple_a_rolled_up_1109180523_refine._simple_a_rolled_up_1109180523_refine'"));
    if (MeshAsset.Succeeded())
    {
        ScrollMesh->SetStaticMesh(MeshAsset.Object);
    }

    SetActorScale3D(FVector(0.2f, 0.2f, 0.2f));
    PrimaryActorTick.bCanEverTick = true;
}

void AASScroll::BeginPlay()
{
    Super::BeginPlay();
}

void AASScroll::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FRotator NewRotation = FRotator(-50.f * DeltaTime, 0.f, 0.f);
    AddActorLocalRotation(NewRotation);
}
