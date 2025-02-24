// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASUserWidget.h"
#include "GameData/ASCharacterStat.h"
#include "ASHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API UASHpBarWidget : public UASUserWidget
{
	GENERATED_BODY()
	
public:
	UASHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FASCharacterStat& BaseStat, const FASCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);
	FString GetHpStatText();

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
