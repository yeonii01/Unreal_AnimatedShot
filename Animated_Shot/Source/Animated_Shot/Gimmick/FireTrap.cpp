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

    FireEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireEffect"));
    FireEffect->SetupAttachment(RootComponent);
    FireEffect->bAutoActivate = false; // ó������ ��Ȱ��ȭ
    FireEffect->SetVisibility(false);

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
            FireEffect->Deactivate();
            FireEffect->SetVisibility(false);
            DamageZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            TimeSinceLastToggle = 0.0f; // Ÿ�̸� �ʱ�ȭ
        }
    }

    else
    {
        if (TimeSinceLastToggle >= FireInactiveTime) // �ð��� ������ ����
        {
            bIsFlameActive = true;
            FireEffect->ActivateSystem();
            FireEffect->SetVisibility(true);
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

