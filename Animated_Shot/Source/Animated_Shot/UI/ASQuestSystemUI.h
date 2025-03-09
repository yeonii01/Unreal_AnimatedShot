// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ASQuestSystemUI.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API UASQuestSystemUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest")
	void UpdateQuestText(const FString& QuestText);
};
