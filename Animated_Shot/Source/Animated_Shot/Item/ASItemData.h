// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ASItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon1 = 0,
	Weapon2,
	Potion,
	Scroll
};
/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API UASItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ASItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Type)
	EItemType Type;

	void SetItemType(EItemType InType) { Type = InType; }
};
