// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/FireTrap.h"
#include "Character/ASCharacterPlayer.h"
#include "Engine/DamageEvents.h"

// Sets default values
AFireTrap::AFireTrap()
{
    PrimaryActorTick.bCanEverTick = true; // Tick ��� �����ϰ� ����

    DamageZone = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageZone"));
    RootComponent = DamageZone;
    DamageZone->SetBoxExtent(FVector(100.f, 50.f, 50.f));
    DamageZone->SetCollisionProfileName(TEXT("Trigger"));
    DamageZone->OnComponentBeginOverlap.AddDynamic(this, &AFireTrap::OnOverlapBegin);
    DamageZone->SetCollisionEnabled(ECollisionEnabled::NoCollision); // ó���� ��Ȱ��ȭ

    FireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticle"));
    FireParticle->SetupAttachment(RootComponent);

    // �⺻ ��ƼŬ �ý��� �ε�
    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Fire/P_flamethrower.P_Flamethrower'"));
    if (ParticleAsset.Succeeded())
    {
        FireParticle->SetTemplate(ParticleAsset.Object);
    }

    // ��ƼŬ �⺻ ����
    FireParticle->bAutoActivate = false;  // ó������ ��Ȱ��ȭ
    FireParticle->SetVisibility(true);    // �⺻������ ���̰� ����
    FireParticle->SetRelativeLocation(FVector(-70, 0, -5)); // ��ġ ����
    FireParticle->SetRelativeRotation(FRotator(-90, 0, 0)); // ��ġ ����
    FireParticle->SetRelativeScale3D(FVector(1.f, 1.f, 1.f)); // ��ġ ����
    FireParticle->SetAbsolute(false, false, false); // �θ��� ������ �޵��� ����
    FireParticle->SetWorldScale3D(FVector(1.f, 1.f, 1.f)); // ��ġ ����

    // �⺻ ���� ����
    bIsFlameActive = false;
    FireActiveTime = 2.0f;
    FireInactiveTime = 3.0f;
    Damage = 30.0f;
    TimeSinceLastToggle = 0.0f;
}

void AFireTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFireTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    // �ð� ������Ʈ
    TimeSinceLastToggle += DeltaTime;


    if (bIsFlameActive)
    {
        if (TimeSinceLastToggle >= FireActiveTime) // �ð��� ������ ����
        {
            bIsFlameActive = false;
            FireParticle->Deactivate();
            FireParticle->SetVisibility(false);
            DamageZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            TimeSinceLastToggle = 0.0f; // Ÿ�̸� �ʱ�ȭ
        }
    }

    else
    {
        if (TimeSinceLastToggle >= FireInactiveTime) // �ð��� ������ ����
        {
            bIsFlameActive = true;
            FireParticle->ActivateSystem();
            FireParticle->SetVisibility(true);
            DamageZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            TimeSinceLastToggle = 0.0f; // Ÿ�̸� �ʱ�ȭ
        }
    }
}

void AFireTrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AASCharacterPlayer* Player = Cast<AASCharacterPlayer>(OtherActor);


    FDamageEvent DamageEvent;

    if (Player) 
        Player->TakeDamage(Damage, DamageEvent, Player->GetController(), this);
}

