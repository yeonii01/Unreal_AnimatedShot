// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/ASCharacterStat.h"
#include "ASCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FASCharacterStat& /*BaseStat*/, const FASCharacterStat& /*ModifierStat*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANIMATED_SHOT_API UASCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UASCharacterStatComponent();

protected:
	virtual void InitializeComponent() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetLevelStat(int32 InNewLevel);
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AddBaseStat(const FASCharacterStat& InAddBaseStat) { BaseStat = BaseStat + InAddBaseStat;  OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetBaseStat(const FASCharacterStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetModifierStat(const FASCharacterStat& InModifierStat) { ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }

	FORCEINLINE const FASCharacterStat& GetBaseStat() const { return BaseStat; }

	FORCEINLINE const FASCharacterStat& GetModifierStat() const { return ModifierStat; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FASCharacterStat GetTotalStat() { return BaseStat + ModifierStat; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	UFUNCTION(BlueprintCallable)
	void HealHp(float InHealAmount);
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetAttackRadius() { return AttackRadius; }
	UFUNCTION(BlueprintCallable)
	float ApplyDamage(float InDamage);

protected:
	void SetHp(float NewHp);

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float AttackRadius;

	UPROPERTY(Transient, VisibleInstanceOnly, Category =Stat, Meta = (AllowPrivateAccess = "true"))
	FASCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FASCharacterStat ModifierStat;
};
