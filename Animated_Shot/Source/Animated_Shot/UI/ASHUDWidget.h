// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/ASCharacterStat.h"
#include "ASHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API UASHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UASHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateStat(const FASCharacterStat& BaseStat, const FASCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UASHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UASCharacterStatWidget> CharacterStat;
};
