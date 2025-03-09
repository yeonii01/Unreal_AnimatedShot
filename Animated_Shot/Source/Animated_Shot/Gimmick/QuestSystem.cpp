// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/QuestSystem.h"
#include "UI/ASHUDWidget.h"
#include "Player/ASPlayerController.h"

// Sets default values
AQuestSystem::AQuestSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CurrentQuest = EQuestType::QUEST_PickUpWeapon;
}

// Called when the game starts or when spawned
void AQuestSystem::BeginPlay()
{
	Super::BeginPlay();
    UpdateQuestUI();
}

// Called every frame
void AQuestSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQuestSystem::AdvanceQuest()
{
    switch (CurrentQuest)
    {
    case EQuestType::QUEST_PickUpWeapon:
        CurrentQuest = EQuestType::QUEST_AttackMonster;
        //UE_LOG(LogTemp, Log, TEXT("����Ʈ 2: ���� �����ϱ� ����"));
        break;

    case EQuestType::QUEST_AttackMonster:
        CurrentQuest = EQuestType::QUEST_UsePotion;
        //UE_LOG(LogTemp, Log, TEXT("����Ʈ 3: ���� ����ϱ� ����"));
        break;

    case EQuestType::QUEST_UsePotion:
        CurrentQuest = EQuestType::QUEST_ReachBossRoom;
        //UE_LOG(LogTemp, Log, TEXT("����Ʈ 4: ������ �����ϱ� ����"));
        break;

    case EQuestType::QUEST_ReachBossRoom:
        CurrentQuest = EQuestType::QUEST_DefeatBoss;
        //UE_LOG(LogTemp, Log, TEXT("����Ʈ 5: ���� ���� óġ�ϱ� ����"));
        break;

    case EQuestType::QUEST_DefeatBoss:
        CurrentQuest = EQuestType::QUEST_Finish;
        //UE_LOG(LogTemp, Log, TEXT("���� Ŭ����! ����Ʈ �Ϸ�!"));
        break;

    default:
        break;
    }

    UpdateQuestUI();
}

bool AQuestSystem::IsQuestCompleted()
{
    return CurrentQuest == EQuestType::QUEST_Finish;
}

void AQuestSystem::UpdateQuestUI()
{
    AASPlayerController* PC = Cast<AASPlayerController>(GetWorld()->GetFirstPlayerController());
    UASHUDWidget* HUDWidget = PC->GetHUDWidget();

    if (HUDWidget)
    {
        FString QuestText;
        switch (CurrentQuest)
        {
        case EQuestType::QUEST_PickUpWeapon:
            QuestText = "Quest 1: Obtain a weapon";
            break;
        case EQuestType::QUEST_AttackMonster:
            QuestText = "Quest 2: Attack monsters while avoiding traps";
            break;
        case EQuestType::QUEST_UsePotion:
            QuestText = "Quest 3: Use a potion to recover";
            break;
        case EQuestType::QUEST_ReachBossRoom:
            QuestText = "Quest 4: Reach the boss room while avoiding fire traps";
            break;
        case EQuestType::QUEST_DefeatBoss:
            QuestText = "Final Quest 5: Defeat the final boss";
            break;
        default:
            QuestText = "No active quest";
            break;
        }

        HUDWidget->UpdateQuestText(QuestText);
    }
}

