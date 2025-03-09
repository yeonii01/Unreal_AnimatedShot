// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ASTriggerQuest.h"
#include "ASTriggerQuest.h"
#include "Physics/ASCollision.h"
#include "Gimmick/QuestSystem.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void AASTriggerQuest::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AASTriggerQuest::OnOverlapBegin);
}

void AASTriggerQuest::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	AQuestSystem* QuestSystem = Cast<AQuestSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), AQuestSystem::StaticClass()));
	if (QuestSystem && QuestSystem->CurrentQuest == EQuestType::QUEST_AttackMonster)
	{
		QuestSystem->AdvanceQuest();
	}

	if (QuestSystem && QuestSystem->CurrentQuest == EQuestType::QUEST_ReachBossRoom)
	{
		QuestSystem->AdvanceQuest();
	}
}
