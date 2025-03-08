// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ASItemData.h"
#include "ASPotionItemData.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API UASPotionItemData : public UASItemData
{
	GENERATED_BODY()
	
public:
	UASPotionItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ASItemData", GetFName());
	}

public:
	UFUNCTION(BlueprintCallable)
	float GetHealAmount() { return HealAmount; }

	UPROPERTY(EditAnywhere, Category = Hp)
	float HealAmount;

};
