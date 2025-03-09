// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UI/ASQuestSystemUI.h"
#include "QuestSystem.generated.h"

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	QUEST_None UMETA(DisplayName = "None"),
	QUEST_PickUpWeapon UMETA(DisplayName = "Pick Up Weapon"),
	QUEST_AttackMonster UMETA(DisplayName = "Attack Monster"),
	QUEST_UsePotion UMETA(DisplayName = "Use Potion"),
	QUEST_ReachBossRoom UMETA(DisplayName = "Reach Boss Room"),
	QUEST_DefeatBoss UMETA(DisplayName = "Defeat Final Boss"),
    QUEST_Finish UMETA(DisplayName = "Finish"),
};

UCLASS()
class ANIMATED_SHOT_API AQuestSystem : public AActor
{
	GENERATED_BODY()
	
public:
    AQuestSystem();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // ���� ����Ʈ �ܰ�
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Quest")
    EQuestType CurrentQuest;

    // UI ������ ���� ����Ʈ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> QuestWidgetClass;

    UPROPERTY()
    UASQuestSystemUI* QuestWidget;

    // ����Ʈ ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Quest")
    void AdvanceQuest();

    // ����Ʈ �Ϸ� ���� üũ
    UFUNCTION(BlueprintCallable, Category = "Quest")
    bool IsQuestCompleted();

    // UI ������Ʈ �Լ�
    UFUNCTION()
    void UpdateQuestUI();
};
