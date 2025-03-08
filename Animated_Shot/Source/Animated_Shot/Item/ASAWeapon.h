// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASItemBase.h"
#include "ASAWeapon.generated.h"

enum class EItemType : uint8;

UCLASS()
class ANIMATED_SHOT_API AASAWeapon : public AASItemBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASAWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetWeaponType(EItemType Type);

	virtual void PickUp(AActor* Actor) override;

public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WeaponMesh;

	bool IsInBox = false;
};