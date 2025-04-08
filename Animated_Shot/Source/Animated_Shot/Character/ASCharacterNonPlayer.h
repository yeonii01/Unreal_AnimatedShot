// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ASCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/ASCharacterAIInterface.h"
#include "ASCharacterNonPlayer.generated.h"

class AASItemBase;
class AASCoin;

/**
 * 
 */
UCLASS(config = AnimatedShot)
class ANIMATED_SHOT_API AASCharacterNonPlayer : public AASCharacterBase, public IASCharacterAIInterface
{
	GENERATED_BODY()

public:
	AASCharacterNonPlayer();

protected:
	virtual void PostInitializeComponents() override;

public:
	bool IsDead() { return Dead; }
	void SetIsDead(bool InDead) { Dead = InDead; }

	void SetNPC(int index);

private:
	bool Dead;
protected:
	virtual void SetDead() override;
	void NPCMeshLoadCompleted();
	void DropItem();
	void DropCoin();


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Settings")
	//TArray<FSoftObjectPath> NPCMeshes;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Settings")
	TArray<USkeletalMesh*> NPCMeshes;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Settings")
	//TSubclassOf<UClass*> AnimClasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Settings")
	TArray<TSubclassOf<UAnimInstance>> AnimClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Settings")
	TArray<UAnimMontage*> DeadMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Settings")
	TArray<UAnimMontage*> ComboActionMontages;

	TSharedPtr<FStreamableHandle> NPCMeshHandle;

	UPROPERTY(EditAnywhere, Category=DropItem)
	uint32 MaxDropItemNum = 3;
	
	UPROPERTY(EditAnywhere, Category=DropItem)
	float MinDropRange = -30.f;
	
	UPROPERTY(EditAnywhere, Category=DropItem)
	float MaxDropRange = 30.f;

	UPROPERTY(EditAnywhere, Category=DropItem)
	TArray<TSubclassOf<AASItemBase>> DropItems;
	
	UPROPERTY(EditAnywhere, Category=DropItem)
	int32 MinCoinValue = 20;
	
	UPROPERTY(EditAnywhere, Category=DropItem)
	int32 MaxCoinValue = 100;

	UPROPERTY(EditAnywhere, Category=DropItem)
	TSubclassOf<AASCoin> CoinClass = nullptr;

public:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	// 머터리얼 인스턴스 동적 변경 가능
	UPROPERTY()
	UMaterialInstanceDynamic* MaskedMaterialInstance;

	void SetCircleColor(FLinearColor NewColor);

protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;

	virtual void NotifyComboActionEnd() override;
public:
	void SetTargetPos(const Protocol::PosInfo& pos);
	void SetObjectId(int64 InObjectId) { ObjectId = InObjectId; };
	int64 GetObjectId() { return ObjectId; };

private:
	int64 ObjectId;
};
