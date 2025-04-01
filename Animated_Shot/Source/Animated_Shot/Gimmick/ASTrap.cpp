// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ASTrap.h"
#include "Components/SphereComponent.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "Character/ASCharacterPlayer.h"
#include "Character/ASPartyCharacterPlayer.h"
#include "Character/ASCharacterBase.h"
#include "Network/ASGameInstance.h"
#include "Engine/DamageEvents.h"

// Sets default values
AASTrap::AASTrap()
{
	PrimaryActorTick.bCanEverTick = true;

    TriggerZone = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerZone"));
    SetRootComponent(TriggerZone);
    TriggerZone->SetSphereRadius(100.0f);
    TriggerZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerZone->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    TriggerZone->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    TriggerZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
    NiagaraEffect->SetupAttachment(RootComponent);
    NiagaraEffect->SetAutoActivate(false);

    CircleEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CircleEffect"));
    CircleEffect->SetupAttachment(RootComponent);
    CircleEffect->SetAutoActivate(false);

    TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AASTrap::OnTriggerEnter);
    TriggerZone->OnComponentEndOverlap.AddDynamic(this, &AASTrap::OnTriggerExit);
}

// Called when the game starts or when spawned
void AASTrap::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AASTrap::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bPlayerOnTrigger)
    {
        CurrentCharge += ChargeSpeed * DeltaTime;
        CurrentCharge = FMath::Clamp(CurrentCharge, 0.0f, 25.f);

        if (CurrentCharge >= 25.f)
        {
            ActivateTrap();
        }
    }
    else
    {
        if (CurrentCharge > 0)
        {
            CurrentCharge -= ChargeSpeed * DeltaTime * 0.5f;
        }
    }

    if (CircleEffect)
    {
        CircleEffect->SetFloatParameter(TEXT("ChargeAmount"), CurrentCharge / 100.0f);
    }
}

int64 AASTrap::GetServerTimeFromInstance()
{
    if (auto* GI = Cast<UASGameInstance>(GWorld->GetGameInstance()))
    {
        return GI->ServerTime;
    }
    return 0;
}

void AASTrap::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AASCharacterBase* Player = Cast<AASCharacterBase>(OtherActor);

    if (Player->IsMyPlayer() || Player->IsPartyPlayer())
    {
        OverlapActor = Player; 
        if (Player->IsMyPlayer())
        {
            DamageActor = Cast<AASCharacterPlayer>(OtherActor);
        }
        bPlayerOnTrigger = true;

        if (CircleEffect)
        {
            CircleEffect->Activate();
        }
    }
 /*   else if (Player->IsPartyPlayer())
    {
        OverlapActor = Cast<AASPartyCharacterPlayer>(Player); 
        bPlayerOnTrigger = true;

        if (CircleEffect)
        {
            CircleEffect->Activate();
        }
    }*/
}

void AASTrap::OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OverlapActor == OtherActor)
    {
        OverlapActor = nullptr;
        bPlayerOnTrigger = false;
    }
}

void AASTrap::ActivateTrap_Implementation()
{
    if (bTrapActivated)
        return;

    bTrapActivated = true;

    if (NiagaraEffect)
    {
        NiagaraEffect->Activate();
    }

    FDamageEvent DamageEvent;

    if (DamageActor)
    {
        DamageActor->TakeDamage(TrapDamage, DamageEvent, DamageActor->GetController(), this);
    }
}
