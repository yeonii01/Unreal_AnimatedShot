// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ASCharacterStatComponent.h"
#include "GameData/ASGameSingleton.h"
#include "ASCharacterStatComponent.h"

// Sets default values for this component's properties
UASCharacterStatComponent::UASCharacterStatComponent()
{
	CurrentLevel = 1;
	AttackRadius = 50.f;
}


// Called when the game starts
void UASCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}

void UASCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UASGameSingleton::Get().CharacterMaxLevel);
	BaseStat = UASGameSingleton::Get().GetCharacterStat(CurrentLevel);
	check(BaseStat.MaxHp > 0.f)
}

float UASCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}

void UASCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}
