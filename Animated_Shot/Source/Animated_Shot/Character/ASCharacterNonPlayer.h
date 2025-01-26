// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ASCharacterBase.h"
#include "ASCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API AASCharacterNonPlayer : public AASCharacterBase
{
	GENERATED_BODY()

public:
	AASCharacterNonPlayer();
	
protected:
	void SetDead() override;
};
