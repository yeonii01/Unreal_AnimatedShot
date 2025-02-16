// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ASGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ASGameMode.h"

AASGameMode::AASGameMode()
{
	//static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	//if (ThirdPersonClassRef.Class)
	//{
	//	DefaultPawnClass = ThirdPersonClassRef.Class;
	//}
	//DefaultPawnClass
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/MyCharacter/AS_Character.AS_Character_C'"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}	

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Animated_Shot.ASPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	static ConstructorHelpers::FObjectFinder<ULevelSequence> SequenceAsset(TEXT("/Game/LS_Title/LS_Title.LS_Title"));
	if (SequenceAsset.Succeeded())
	{
		IntroSequence = SequenceAsset.Object;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/UI/WBP_Title.WBP_Title_C"));
	if (WidgetClass.Succeeded())
	{
		TextWidgetClass = WidgetClass.Class;
	}

	TextWidget = nullptr;
}

void AASGameMode::BeginPlay()
{
	Super::BeginPlay();
	PlayIntroSequence();
	if (TextWidgetClass)
	{
		TextWidget = CreateWidget<UUserWidget>(GetWorld(), TextWidgetClass);
		if (TextWidget) TextWidget->AddToViewport();
	}
}

void AASGameMode::PlayIntroSequence()
{
	ULevelSequence* Sequence = IntroSequence.LoadSynchronous();
	if (Sequence)
	{
		FMovieSceneSequencePlaybackSettings PlaybackSettings;
		PlaybackSettings.bAutoPlay = true;

		ALevelSequenceActor* SequenceActor;
		ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence, PlaybackSettings, SequenceActor);

		if (SequencePlayer)
		{
			SequencePlayer->OnFinished.AddDynamic(this, &AASGameMode::OnSequenceFinished);
			SequencePlayer->Play();
		}
	}
}

void AASGameMode::OnSequenceFinished()
{
	TextWidget->RemoveFromParent();
	TextWidget = nullptr;
}