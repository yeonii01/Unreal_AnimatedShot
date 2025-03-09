// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/FireTrap.h"
#include "Character/ASCharacterPlayer.h"
#include "Engine/DamageEvents.h"

// Sets default values
AFireTrap::AFireTrap()
{
    PrimaryActorTick.bCanEverTick = true; // Tick 사용 가능하게 설정

    DamageZone = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageZone"));
    RootComponent = DamageZone;
    DamageZone->SetBoxExtent(FVector(100.f, 50.f, 50.f));
    DamageZone->SetCollisionProfileName(TEXT("Trigger"));
    DamageZone->OnComponentBeginOverlap.AddDynamic(this, &AFireTrap::OnOverlapBegin);
    DamageZone->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 처음엔 비활성화

    FireEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireEffect"));
    FireEffect->SetupAttachment(RootComponent);
    FireEffect->bAutoActivate = false; // 처음에는 비활성화
    FireEffect->SetVisibility(false);

    // 기본 변수 설정
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
    // 시간 업데이트
    TimeSinceLastToggle += DeltaTime;


    if (bIsFlameActive)
    {
        if (TimeSinceLastToggle >= FireActiveTime) // 시간이 지나면 꺼짐
        {
            bIsFlameActive = false;
            FireEffect->Deactivate();
            FireEffect->SetVisibility(false);
            DamageZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            TimeSinceLastToggle = 0.0f; // 타이머 초기화
        }
    }

    else
    {
        if (TimeSinceLastToggle >= FireInactiveTime) // 시간이 지나면 켜짐
        {
            bIsFlameActive = true;
            FireEffect->ActivateSystem();
            FireEffect->SetVisibility(true);
            DamageZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            TimeSinceLastToggle = 0.0f; // 타이머 초기화
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

