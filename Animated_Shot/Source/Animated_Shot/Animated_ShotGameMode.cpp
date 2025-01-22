// Copyright Epic Games, Inc. All Rights Reserved.

#include "Animated_ShotGameMode.h"
#include "Animated_ShotCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAnimated_ShotGameMode::AAnimated_ShotGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
