// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Blueprint/UserWidget.h"
#include "ASGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API AASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AASGameMode();

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Cinematic")
	TSoftObjectPtr<ULevelSequence> IntroSequence;

	// ���� Ŭ���� ����
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> TextWidgetClass;

	void PlayIntroSequence();

	UFUNCTION()
	void OnSequenceFinished();

private:
	UUserWidget* TextWidget;
};
