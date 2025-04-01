// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/FireTrap.h"
#include "Character/ASCharacterPlayer.h"
#include "Network/ASGameInstance.h"
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

    FireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticle"));
    FireParticle->SetupAttachment(RootComponent);

    // 기본 파티클 시스템 로드
    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Fire/P_flamethrower.P_Flamethrower'"));
    if (ParticleAsset.Succeeded())
    {
        FireParticle->SetTemplate(ParticleAsset.Object);
    }

    // 파티클 기본 설정
    FireParticle->bAutoActivate = false;  // 처음에는 비활성화
    FireParticle->SetVisibility(true);    // 기본적으로 보이게 설정
    FireParticle->SetRelativeLocation(FVector(-70, 0, -5)); // 위치 조정
    FireParticle->SetRelativeRotation(FRotator(-90, 0, 0)); // 위치 조정
    FireParticle->SetRelativeScale3D(FVector(1.f, 1.f, 1.f)); // 위치 조정
    FireParticle->SetAbsolute(false, false, false); // 부모의 영향을 받도록 설정
    FireParticle->SetWorldScale3D(FVector(1.f, 1.f, 1.f)); // 위치 조정

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

int64 AFireTrap::GetServerTimeFromInstance()
{
    if (auto* GI = Cast<UASGameInstance>(GWorld->GetGameInstance()))
    {
        return GI->ServerTime;
    }
    return 0;
}

void AFireTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    int64 Now = GetServerTimeFromInstance();

    if (TimeSinceLastToggle == 0)
    {
        TimeSinceLastToggle = Now;
        return;
    }

    int64 Elapsed = Now - TimeSinceLastToggle;

    if (bIsFlameActive)
    {
        if (Elapsed >= static_cast<int64>(FireActiveTime * 1000)) 
        {
            bIsFlameActive = false;
            FireParticle->Deactivate();
            FireParticle->SetVisibility(false);
            DamageZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            TimeSinceLastToggle = Now; 
        }
    }

    else
    {
        if (Elapsed >= static_cast<int64>(FireActiveTime * 1000)) 
        {
            bIsFlameActive = true;
            FireParticle->ActivateSystem();
            FireParticle->SetVisibility(true);
            DamageZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            TimeSinceLastToggle = Now; 
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

