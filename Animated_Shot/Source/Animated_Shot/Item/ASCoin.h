// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASCoin.generated.h"

UCLASS()
class ANIMATED_SHOT_API AASCoin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASCoin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetCoin(int32 CoinValue) { Coin = CoinValue; }

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Coin = 0;
};
