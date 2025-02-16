// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Sound/SoundCue.h"
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

	// 위젯 클래스 지정
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> TextWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Audio")
	TSoftObjectPtr<USoundCue> BackgroundMusic;

	void PlayBackgroundMusic();
	void PlayIntroSequence();

	UFUNCTION()
	void OnSequenceFinished();

private:
	UUserWidget* TextWidget;
};
