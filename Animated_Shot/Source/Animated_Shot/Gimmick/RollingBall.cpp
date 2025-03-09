// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/RollingBall.h"
#include "Components/StaticMeshComponent.h"

ARollingBall::ARollingBall()
{
    PrimaryActorTick.bCanEverTick = true;

    // ��ü �޽� ����
    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    RootComponent = BallMesh;

    // ���� �ùķ��̼� Ȱ��ȭ
    BallMesh->SetSimulatePhysics(true);
    BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BallMesh->SetCollisionObjectType(ECC_PhysicsBody);

    // ���� ���� (�߷� ����)
    BallMesh->SetMassOverrideInKg(NAME_None, 1000.0f); // ���� ���� ����
    BallMesh->BodyInstance.AngularDamping = 0.01f;   // ȸ�� ���� ����
    BallMesh->BodyInstance.LinearDamping = 0.05f;    // ���� ���� ����

    // ���� �� �ڿ������� �������� ź�� ����
    BallMesh->SetPhysMaterialOverride(nullptr); // ���߿� ���� ���� ���� ����
}

// Called when the game starts or when spawned
void ARollingBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARollingBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

