// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ASItemData.h"
#include "GameData/ASCharacterStat.h"
#include "ASWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API UASWeaponItemData : public UASItemData
{
	GENERATED_BODY()

public:
	UASWeaponItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ASItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<UStaticMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, Category =Stat)
	FASCharacterStat ModifierStat;
};
