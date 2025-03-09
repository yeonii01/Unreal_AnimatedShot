// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ASTriggerQuest.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API AASTriggerQuest : public ATriggerBox
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION()
    void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
};
