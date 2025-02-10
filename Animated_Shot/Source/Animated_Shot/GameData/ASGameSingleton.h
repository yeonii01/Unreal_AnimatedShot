// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ASCharacterStat.h"
#include "ASGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogASGameSingleton, Error, All);
/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API UASGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UASGameSingleton();
	static UASGameSingleton& Get();

	//Character Stat Data Section
public:
	FORCEINLINE FASCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel) ? CharacterStatTable[InLevel] : FASCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	TArray<FASCharacterStat> CharacterStatTable;
};
