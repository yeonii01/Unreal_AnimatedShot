// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ASPlayerController.h"
#include "UI/ASHUDWidget.h"

AASPlayerController::AASPlayerController()
{
	static ConstructorHelpers::FClassFinder<UASHUDWidget> ASHUDWidgetRef(TEXT("/Game/UI/WBP_ASHUD.WBP_ASHUD_C"));
	if (ASHUDWidgetRef.Class)
	{
		ASHUDWidgetClass = ASHUDWidgetRef.Class;
	}
}

void AASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	ASHUDWidget = CreateWidget<UASHUDWidget>(this, ASHUDWidgetClass);
	if (ASHUDWidget)
	{
		ASHUDWidget->AddToViewport();
	}
}
