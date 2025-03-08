// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ASItemBase.h"
#include "ASScroll.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API AASScroll : public AASItemBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AASScroll();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ScrollMesh;
};
