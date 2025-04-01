// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ASSpawner.h"
#include "Components/StaticMeshComponent.h"
#include "Character/ASCharacterNonPlayer.h"
#include "Engine/OverlapResult.h"
#include "Item/ASItemBox.h"
#include "Gimmick/QuestSystem.h"
#include "Kismet/GameplayStatics.h"


AASSpawner::AASSpawner()
{
	//Stage Stat
	CurrentStageNum = 1;
	//PrimaryActorTick.bCanEverTick = true;
}

void AASSpawner::BeginPlay()
{
	Super::BeginPlay();

	OnOpponentSpawn();
}

void AASSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeAccumulator += DeltaTime;

	if (TimeAccumulator >= ActionInterval)
	{
		if (ASOpponentCharacter)
		{
			Protocol::PosInfo pos;
			pos.set_x(0);
			pos.set_y(0);
			pos.set_z(0);
			ASOpponentCharacter->SetTargetPos(pos);
			TimeAccumulator = 0.0f;
		}
	}
}

void AASSpawner::OnOpponentDestroyed(AActor* DestroyedActor)
{
	AQuestSystem* QuestSystem = Cast<AQuestSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), AQuestSystem::StaticClass()));
	if (QuestSystem && QuestSystem->CurrentQuest == EQuestType::QUEST_DefeatBoss)
	{
		QuestSystem->AdvanceQuest();
	}
}

void AASSpawner::OnOpponentSpawn()
{
	FRotator SpawnRotation = FRotator(90.f, 0.f, 0.f);
	const FTransform SpawnTransform(SpawnRotation, GetActorLocation() + FVector::UpVector * 88.f);
	 ASOpponentCharacter = GetWorld()->SpawnActorDeferred<AASCharacterNonPlayer>(OpponentClass, SpawnTransform);
	if (ASOpponentCharacter)
	{
		ASOpponentCharacter->OnDestroyed.AddDynamic(this, &AASSpawner::OnOpponentDestroyed);
		ASOpponentCharacter->SetLevel(CurrentStageNum);	
		ASOpponentCharacter->FinishSpawning(SpawnTransform);
	}
}

