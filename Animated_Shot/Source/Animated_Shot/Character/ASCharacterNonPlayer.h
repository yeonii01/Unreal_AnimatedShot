// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ASCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/ASCharacterAIInterface.h"
#include "ASCharacterNonPlayer.generated.h"

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

protected:
	virtual void SetDead() override;
	void NPCMeshLoadCompleted();

	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;

	TArray<UClass*> AnimClasses;
	TArray<UAnimMontage*> DeadMontages;
	TArray<UAnimMontage*> ComboActionMontages;

	TSharedPtr<FStreamableHandle> NPCMeshHandle;

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
};
