// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ASCharacterBase.h"
#include "Item/AASItemWeaponBox.h"
#include "ASPartyCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API AASPartyCharacterPlayer : public AASCharacterBase
{
	GENERATED_BODY()

public:
	AASPartyCharacterPlayer();

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;

public:
	void PlayDamageMontage();
	void EquipWeapon(bool index);

	void SetWeaponBox(AAASItemWeaponBox* InWeaponBox) { WeaponBox = InWeaponBox; }
private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DamageMontage;

private:
	AAASItemWeaponBox* WeaponBox;
};
