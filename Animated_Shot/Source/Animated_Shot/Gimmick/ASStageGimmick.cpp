// Fill out your copyright notice in the Description page of Project Settings.


#include "ASStageGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/ASCollision.h"
#include "Character/ASCharacterNonPlayer.h"
#include "Engine/OverlapResult.h"
#include "Item/ASItemBox.h"

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
	Gate->SetRelativeLocation(FVector(-1220.f, -230.5f, 20.f));
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

	//Fight Section
	OpponentSpawnTime = 2.f;
	OpponentClass = AASCharacterNonPlayer::StaticClass();

	//Reward Section
	RewardBoxClass = AASItemBox::StaticClass();
	FVector BoxLocation = Stage->GetSocketLocation(GateSocket) / 2;
	RewardBoxLocations.Add(GateSocket, BoxLocation);
}

void AASStageGimmick::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
		FString::Printf(TEXT("%d %d %d"), GetActorLocation().X, GetActorLocation().Y,
			GetActorLocation().Z));

	CurrentState = EStageState::READY;
	StateChangeActions.Add(EStageState::READY, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AASStageGimmick::SetReady)));
	StateChangeActions.Add(EStageState::FIGHT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AASStageGimmick::SetFight)));
	StateChangeActions.Add(EStageState::REWARD, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AASStageGimmick::SetChooseReward)));
	StateChangeActions.Add(EStageState::NEXT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AASStageGimmick::SetChooseNext)));
}

//void AASStageGimmick::OnConstruction(const FTransform& Transform)
//{
//	/*Super::OnConstruction(Transform);
//
//	SetState(CurrentState);*/
//}

void AASStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetState(EStageState::FIGHT);
}

void AASStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//
}

void AASStageGimmick::OpenGates()
{
	for (auto& Gate : Gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		(Gate.Value)->SetRelativeLocation(FVector(-1220.f, -230.5f, 20.f));
	}
}

void AASStageGimmick::CloseGates()
{
	for (auto& Gate : Gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		(Gate.Value)->SetRelativeLocation(FVector(-1270.f, -300.5f, 20.f));
	}
}

void AASStageGimmick::SetState(EStageState InNewState)
{
	if (StateChangeActions.Contains(InNewState))
	{
		StateChangeActions[InNewState].StageDelegate.ExecuteIfBound();
	}
}

void AASStageGimmick::SetReady()
{
	StageTrigger->SetCollisionProfileName(CPROFILE_ASTRIGGER);

	for (auto& GateTrigger : GateTriggers) GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	OpenGates();
}

void AASStageGimmick::SetFight()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	for (auto& GateTrigger : GateTriggers) GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
		FString::Printf(TEXT("%d %d %d"), GetActorLocation().X, GetActorLocation().Y,
			GetActorLocation().Z));

	CloseGates();

	OnOpponentSpawn();
}

void AASStageGimmick::SetChooseReward()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	for (auto& GateTrigger : GateTriggers) GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	CloseGates();
	SpawnRewardBoxes();
}

void AASStageGimmick::SetChooseNext()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	for (auto& GateTrigger : GateTriggers) GateTrigger->SetCollisionProfileName(CPROFILE_ASTRIGGER);

	OpenGates();
}

void AASStageGimmick::OnOpponentDestroyed(AActor* DestroyedActor)
{
	SetState(EStageState::REWARD);
}

void AASStageGimmick::OnOpponentSpawn()
{
	const FVector SpawnLocation = GetActorLocation() + FVector::UpVector * 88.f;
	AActor* OpponentActor = GetWorld()->SpawnActor(OpponentClass, &SpawnLocation, &FRotator::ZeroRotator);
	AASCharacterNonPlayer* ASOpponentCharacter = Cast<AASCharacterNonPlayer>(OpponentActor);
	if (ASOpponentCharacter)
	{
		ASOpponentCharacter->OnDestroyed.AddDynamic(this, &AASStageGimmick::OnOpponentDestroyed);
	}
}

void AASStageGimmick::OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (const auto& RewardBox : RewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			AASItemBox* ValidItemBox = RewardBox.Get();
			AActor* OverlappedBox = OverlappedComponent->GetOwner();
			if (OverlappedBox != ValidItemBox)
			{
				ValidItemBox->Destroy();
			}
		}
	}
	SetState(EStageState::NEXT);
}

void AASStageGimmick::SpawnRewardBoxes()
{
	for (const auto& RewardBoxLocation : RewardBoxLocations)
	{
		FVector WorldSpawnLocation = GetActorLocation() + RewardBoxLocation.Value + FVector(0.f, 0.f, 30.f);
		AActor* ItemActor = GetWorld()->SpawnActor(RewardBoxClass, &WorldSpawnLocation, &FRotator::ZeroRotator);
		AASItemBox* RewardBoxActor = Cast<AASItemBox>(ItemActor);
		if (RewardBoxActor)
		{
			RewardBoxActor->Tags.Add(RewardBoxLocation.Key);
			RewardBoxActor->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this,&AASStageGimmick::OnRewardTriggerBeginOverlap);
			RewardBoxes.Add(RewardBoxActor);
		}
	}
}
