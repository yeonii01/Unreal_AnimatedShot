// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/RollingBall.h"
#include "Components/StaticMeshComponent.h"
#include "Character/ASCharacterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

ARollingBall::ARollingBall()
{
    PrimaryActorTick.bCanEverTick = true;

    // 나뭇가지 메쉬 생성
    BranchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BranchMesh"));
    RootComponent = BranchMesh;

    // 물리 적용 활성화
    BranchMesh->SetSimulatePhysics(true);
    BranchMesh->SetCollisionProfileName(TEXT("PhysicsActor"));

    // ProjectileMovementComponent 추가 (중력 적용)
    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
    MovementComponent->bShouldBounce = false;  // 튀지 않도록 설정
    MovementComponent->ProjectileGravityScale = 1.0f; // 중력 적용

    // 충돌 이벤트 바인딩
    BranchMesh->OnComponentHit.AddDynamic(this, &ARollingBall::OnHit);
}

// Called when the game starts or when spawned
void ARollingBall::BeginPlay()
{
	Super::BeginPlay();

    InitialLocation = GetActorLocation();
    InitialRotator = GetActorRotation();
}

// Called every frame
void ARollingBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector CurrentLocation = GetActorLocation();
    float DistanceMoved = FVector::Dist(InitialLocation, CurrentLocation);

    // 일정 거리 이상 이동하면 삭제
    if (DistanceMoved >= DestroyDistance)
    {
        ResetBranch();
    }
}

void ARollingBall::ResetBranch()
{
    SetActorLocation(InitialLocation);
    SetActorRotation(InitialRotator);
    BranchMesh->SetPhysicsLinearVelocity(FVector::ZeroVector); // 속도 초기화
    BranchMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector); // 회전 초기화
}

void ARollingBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        // 플레이어가 맞았을 경우 데미지 적용
        AASCharacterPlayer* Player = Cast<AASCharacterPlayer>(OtherActor);
        if (Player)
        {
            FDamageEvent DamageEvent;
            Player->TakeDamage(DamageAmount, DamageEvent, Player->GetController(), this);
        }

        // 충돌 시 파티클 생성
        if (ImpactParticle)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, GetActorLocation());
        }
        ResetBranch();
    }
}

