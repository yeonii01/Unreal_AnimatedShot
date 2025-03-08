// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ASCoin.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
AASCoin::AASCoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AASCoin::BeginPlay()
{
	Super::BeginPlay();

	Coin = FMath::RandRange(10, 70);
}

void AASCoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRotation = FRotator(0.f, -50.f * DeltaTime, 0.f);
	AddActorLocalRotation(NewRotation);
}
