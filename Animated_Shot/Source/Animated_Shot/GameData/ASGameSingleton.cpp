// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ASGameSingleton.h"

DEFINE_LOG_CATEGORY(LogASGameSingleton);

UASGameSingleton::UASGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/GameData/ASCharacterStatTable.ASCharacterStatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FASCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UASGameSingleton& UASGameSingleton::Get()
{
	UASGameSingleton* Singleton = CastChecked<UASGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogASGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UASGameSingleton>();
}
