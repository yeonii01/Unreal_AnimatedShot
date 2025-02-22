// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/ASCharacterStat.h"
#include "ASCharacterStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API UASCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FASCharacterStat& BaseStat, const FASCharacterStat& ModifierStat);
	
private:
	UPROPERTY()
	TMap<FName, class UTextBlock*> BaseLookup;

	UPROPERTY()
	TMap<FName, class UTextBlock*> ModifierLookup;
};
