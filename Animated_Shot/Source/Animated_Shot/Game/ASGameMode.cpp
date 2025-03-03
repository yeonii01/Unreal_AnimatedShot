// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ASGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
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

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/UI/WBP_Start.WBP_Start_C"));
	if (WidgetClass.Succeeded())
	{
		TextWidgetClass = WidgetClass.Class;
	}

	/*static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/InventorySystem/UI/WBP_Inventory.WBP_Inventory_C"));
	if (WidgetClass.Succeeded())
	{
		TextWidgetClass = WidgetClass.Class;
	}*/
	static ConstructorHelpers::FObjectFinder<USoundCue> BGMAsset(TEXT("/Game/Sound/AS_Bgm.AS_Bgm"));
	if (BGMAsset.Succeeded())
	{
		BackgroundMusic = BGMAsset.Object;
	}

	TextWidget = nullptr;
}

void AASGameMode::BeginPlay()
{
	Super::BeginPlay();
	//FSoftObjectPath SequencePath(TEXT("/Game/LS_Title/LS_Title.LS_Title"));
	//IntroSequence = Cast<ULevelSequence>(StaticLoadObject(ULevelSequence::StaticClass(), nullptr, *SequencePath.ToString()));

	//PlayIntroSequence();
	if (TextWidgetClass)
	{
		TextWidget = CreateWidget<UUserWidget>(GetWorld(), TextWidgetClass);
		if (TextWidget) TextWidget->AddToViewport(100);
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly()); 
	}

	StartButton = Cast<UButton>(TextWidget->GetWidgetFromName(TEXT("StartButton")));
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &AASGameMode::OnStartButtonClicked);
	}

	PlayBackgroundMusic();
}

void AASGameMode::PlayBackgroundMusic()
{
	UGameplayStatics::PlaySound2D(GetWorld(), BackgroundMusic.Get());
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

void AASGameMode::OnStartButtonClicked()
{
	if (TextWidget)
	{
		TextWidget->RemoveFromParent();
		TextWidget = nullptr;
	}

	// 마우스 커서 숨기기 및 입력 모드 변경
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}
