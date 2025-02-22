// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ASItemData.h"
#include "GameData/ASCharacterStat.h"
#include "ASScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API UASScrollItemData : public UASItemData
{
	GENERATED_BODY()

public:
	UASScrollItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ASItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Stat)
	FASCharacterStat BaseStat;
};
