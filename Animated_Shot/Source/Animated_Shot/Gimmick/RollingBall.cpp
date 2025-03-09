// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/RollingBall.h"
#include "Components/StaticMeshComponent.h"
#include "Character/ASCharacterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

ARollingBall::ARollingBall()
{
    PrimaryActorTick.bCanEverTick = true;

    // �������� �޽� ����
    BranchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BranchMesh"));
    RootComponent = BranchMesh;

    // ���� ���� Ȱ��ȭ
    BranchMesh->SetSimulatePhysics(true);
    BranchMesh->SetCollisionProfileName(TEXT("PhysicsActor"));

    // ProjectileMovementComponent �߰� (�߷� ����)
    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
    MovementComponent->bShouldBounce = false;  // Ƣ�� �ʵ��� ����
    MovementComponent->ProjectileGravityScale = 1.0f; // �߷� ����

    // �浹 �̺�Ʈ ���ε�
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

    // ���� �Ÿ� �̻� �̵��ϸ� ����
    if (DistanceMoved >= DestroyDistance)
    {
        ResetBranch();
    }
}

void ARollingBall::ResetBranch()
{
    SetActorLocation(InitialLocation);
    SetActorRotation(InitialRotator);
    BranchMesh->SetPhysicsLinearVelocity(FVector::ZeroVector); // �ӵ� �ʱ�ȭ
    BranchMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector); // ȸ�� �ʱ�ȭ
}

void ARollingBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        // �÷��̾ �¾��� ��� ������ ����
        AASCharacterPlayer* Player = Cast<AASCharacterPlayer>(OtherActor);
        if (Player)
        {
            FDamageEvent DamageEvent;
            Player->TakeDamage(DamageAmount, DamageEvent, Player->GetController(), this);
        }

        // �浹 �� ��ƼŬ ����
        if (ImpactParticle)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, GetActorLocation());
        }
        ResetBranch();
    }
}

