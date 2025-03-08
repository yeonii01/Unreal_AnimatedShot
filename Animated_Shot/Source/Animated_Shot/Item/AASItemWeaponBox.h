// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ASItemBox.h"
#include "AASItemWeaponBox.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API AAASItemWeaponBox : public AASItemBox
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAASItemWeaponBox();

	virtual void BeginPlay() override;
protected:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult) override;
	virtual void OnEffectFinished(class UParticleSystemComponent* ParticleSystem) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AASAWeapon> WeaponClass;

	TArray<AASAWeapon*> SpawnWeapons;

	UFUNCTION()
	void OpenBox();

	UFUNCTION()
	void SelectWeapon(int _key);
	
};
