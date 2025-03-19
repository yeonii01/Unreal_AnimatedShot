// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASPartyCharacterPlayer.h"

AASPartyCharacterPlayer::AASPartyCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AASPartyCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

}

void AASPartyCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

