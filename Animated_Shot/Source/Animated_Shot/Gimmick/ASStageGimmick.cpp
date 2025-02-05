// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ASStageGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/ASCollision.h"

// Sets default values
AASStageGimmick::AASStageGimmick()
{
	//StageSection
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>StageMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/CemeteryDungeon/Meshes/Floor/SM_cdfloor6.SM_cdfloor6'"));
	if (StageMeshRef.Object)
	{
		Stage->SetStaticMesh(StageMeshRef.Object);
	}

	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetBoxExtent(FVector(775.f, 775.f, 300.f));
	StageTrigger->SetupAttachment(Stage);
	StageTrigger->SetRelativeLocation(FVector(0.f, 0.f, 250.f));
	StageTrigger->SetCollisionProfileName(CPROFILE_ASTRIGGER);
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AASStageGimmick::OnStageTriggerBeginOverlap);

	static FName GateSocket = TEXT("Gate");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/CemeteryDungeon/Meshes/Door/SM_cddoor2.SM_cddoor2'"));
	UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);
	Gate->SetStaticMesh(GateMeshRef.Object);
	Gate->SetupAttachment(Stage, GateSocket);
	Gate->SetRelativeLocation(FVector(-1220.f, -230.5f, 10.f));
	Gate->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	Gates.Add(GateSocket, Gate);

	FName TriggerName = *GateSocket.ToString().Append(TEXT("Trigger"));
	UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);
	GateTrigger->SetBoxExtent(FVector(100.f, 100.f, 300.f));
	GateTrigger->SetupAttachment(Stage, GateSocket);
	GateTrigger->SetRelativeLocation(FVector(-1220.f, -230.5f, 250.f));
	GateTrigger->SetCollisionProfileName(CPROFILE_ASTRIGGER);
	GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AASStageGimmick::OnGateTriggerBeginOverlap);
	GateTrigger->ComponentTags.Add(GateSocket);

	GateTriggers.Add(GateTrigger);

	CurrentState = EStageState::READY;
	StateChangeActions.Add(EStageState::READY, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AASStageGimmick::SetReady)));
	StateChangeActions.Add(EStageState::FIGHT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AASStageGimmick::SetFight)));
	StateChangeActions.Add(EStageState::REWARD, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AASStageGimmick::SetChooseReward)));
	StateChangeActions.Add(EStageState::NEXT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AASStageGimmick::SetChooseNext)));
}

void AASStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AASStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AASStageGimmick::OpenGates()
{
}

void AASStageGimmick::CloseGates()
{
}

void AASStageGimmick::SetState(EStageState InNewState)
{
	CurrentState = InNewState;

	if (StateChangeActions.Contains(InNewState))
	{
		StateChangeActions[CurrentState].StageDelegate.ExecuteIfBound();
	}
}

void AASStageGimmick::SetReady()
{
}

void AASStageGimmick::SetFight()
{
}

void AASStageGimmick::SetChooseReward()
{
}

void AASStageGimmick::SetChooseNext()
{
}