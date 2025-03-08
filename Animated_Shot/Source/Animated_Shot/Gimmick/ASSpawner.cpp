// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ASSpawner.h"
#include "Components/StaticMeshComponent.h"
#include "Character/ASCharacterNonPlayer.h"
#include "Engine/OverlapResult.h"
#include "Item/ASItemBox.h"

// Sets default values
AASSpawner::AASSpawner()
{
	//Stage Stat
	CurrentStageNum = 1;
}

// Called when the game starts or when spawned
void AASSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AASSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AASSpawner::OnOpponentDestroyed(AActor* DestroyedActor)
{
}

void AASSpawner::OnOpponentSpawn()
{
	FRotator SpawnRotation = FRotator(0.f, -90.f, 0.f);
	const FTransform SpawnTransform(SpawnRotation, GetActorLocation() + FVector::UpVector * 88.f);
	AASCharacterNonPlayer* ASOpponentCharacter = GetWorld()->SpawnActorDeferred<AASCharacterNonPlayer>(OpponentClass, SpawnTransform);
	if (ASOpponentCharacter)
	{
		ASOpponentCharacter->OnDestroyed.AddDynamic(this, &AASSpawner::OnOpponentDestroyed);
		//CurrentStageNum = 2;								//�����ؾ���
		ASOpponentCharacter->SetLevel(CurrentStageNum);		//npc ���� ����
		ASOpponentCharacter->FinishSpawning(SpawnTransform);
	}
}

