// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ASAnimationAttackInterface.h"
#include "Interface/ASCharacterWidgetInterface.h"
#include "Interface/ASCharacterItemInterface.h"
#include "GameData/ASCharacterStat.h"
#include "ASCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogASCharacter, Log, All);

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UASItemData* /*InItemData*/)
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
	FOnTakeItemDelegate ItemDelegate;
};

UCLASS()
class ANIMATED_SHOT_API AASCharacterBase : public ACharacter, public IASAnimationAttackInterface, public IASCharacterWidgetInterface, public IASCharacterItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AASCharacterBase();

	virtual void PostInitializeComponents() override;
	
	UFUNCTION(BlueprintCallable)
	bool IsEquipWeapon();

protected:
	virtual void SetCharacterControlData(const class UASCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UASCharacterControlData*> CharacterControlManager;

//Combo Action Section
protected:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = Attacck, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UASComboActionData> ComboActionData;

	void ProcessComboCommand();

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	virtual void NotifyComboActionEnd();
	void SetComboCheckTimer();
	void ComboCheck();

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

//Attack Hit Section
protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

//Dead Section
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	virtual void PlayDeadAnimation();

	float DeadEventDelayTime = 5.f;

//Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UASCharacterStatComponent> Stat;

//UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> HpBar;

	virtual void SetUpCharacterWidget(class UASUserWidget* InUserWidget) override;

//Item Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Weapon1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Weapon2;

	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	virtual void TakeItem(class UASItemData* InItemData) override;
	virtual void DrinkPotion(class UASItemData* InItemData);
	virtual void EquipWeapon(class UASItemData* InItemData);
	virtual void ReadScroll(class UASItemData* InItemData);

//Stat Section
public:
	int32 GetLevel();
	void SetLevel(int32 InNewLevel);
	void ApplyStat(const FASCharacterStat& BaseStat, const FASCharacterStat& ModifierStat);

};