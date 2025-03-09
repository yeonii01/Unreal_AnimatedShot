// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/RollingBall.h"
#include "Components/StaticMeshComponent.h"

ARollingBall::ARollingBall()
{
    PrimaryActorTick.bCanEverTick = true;

    // 구체 메시 생성
    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    RootComponent = BallMesh;

    // 물리 시뮬레이션 활성화
    BallMesh->SetSimulatePhysics(true);
    BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BallMesh->SetCollisionObjectType(ECC_PhysicsBody);

    // 물리 설정 (중력 적용)
    BallMesh->SetMassOverrideInKg(NAME_None, 1000.0f); // 공의 무게 설정
    BallMesh->BodyInstance.AngularDamping = 0.01f;   // 회전 감속 설정
    BallMesh->BodyInstance.LinearDamping = 0.05f;    // 직선 감속 설정

    // 공이 더 자연스럽게 구르도록 탄성 적용
    BallMesh->SetPhysMaterialOverride(nullptr); // 나중에 물리 재질 설정 가능
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

