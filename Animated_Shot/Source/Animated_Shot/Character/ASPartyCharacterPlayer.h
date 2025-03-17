// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ASCharacterBase.h"
#include "ASPartyCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API AASPartyCharacterPlayer : public AASCharacterBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay();
};
