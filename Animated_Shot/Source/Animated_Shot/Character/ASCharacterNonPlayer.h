// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ASCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "ASCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS(config=AnimatedShot)
class ANIMATED_SHOT_API AASCharacterNonPlayer : public AASCharacterBase
{
	GENERATED_BODY()

public:
	AASCharacterNonPlayer();
	
protected:
	virtual void PostInitializeComponents() override;

protected:
	virtual void SetDead() override;
	void NPCMeshLoadCompleted();

	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;

	TArray<UClass*> AnimClasses;
	TArray<UAnimMontage*> DeadMontages;

	TSharedPtr<FStreamableHandle> NPCMeshHandle;
};
