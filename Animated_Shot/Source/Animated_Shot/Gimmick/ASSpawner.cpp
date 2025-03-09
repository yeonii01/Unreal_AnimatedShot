// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ASSpawner.h"
#include "Components/StaticMeshComponent.h"
#include "Character/ASCharacterNonPlayer.h"
#include "Engine/OverlapResult.h"
#include "Item/ASItemBox.h"
#include "Gimmick/QuestSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AASSpawner::AASSpawner()
{
	//Stage Stat
	CurrentStageNum = 1;
}

// Called when the game starts or when spawned
void AASSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	OnOpponentSpawn();
}

// Called every frame
void AASSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	FRotator SpawnRotation = FRotator(0.f, -90.f, 0.f);
	const FTransform SpawnTransform(SpawnRotation, GetActorLocation() + FVector::UpVector * 88.f);
	AASCharacterNonPlayer* ASOpponentCharacter = GetWorld()->SpawnActorDeferred<AASCharacterNonPlayer>(OpponentClass, SpawnTransform);
	if (ASOpponentCharacter)
	{
		ASOpponentCharacter->OnDestroyed.AddDynamic(this, &AASSpawner::OnOpponentDestroyed);
		ASOpponentCharacter->SetLevel(CurrentStageNum);		//npc 레벨 설정
		ASOpponentCharacter->FinishSpawning(SpawnTransform);
	}
}

