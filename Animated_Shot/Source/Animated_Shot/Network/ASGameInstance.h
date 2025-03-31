// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Animated_Shot.h"
#include "Character/ASCharacterBase.h"
#include "Character/ASPartyCharacterPlayer.h"
#include "Character/ASCharacterNonPlayer.h"
#include "ASGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API UASGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UASGameInstance();

public:
	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void DisConnectFromGameServer();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);

public:
	void HandleSpawn(const Protocol::ObjectInfo& PlayerInfo, bool IsMine);
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);

	void HandleDespawn(uint64 ObjectId);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);

	void HandleMove(const Protocol::S_MOVE& MovePkt);
	void HandleMonsterMove(const Protocol::S_MONSTER_MOVE& MonsterPkt);

	void HandleRegisterWeapon(const Protocol::S_PARTY_WEAPON& WeaponPkt);
public:
	//GameServer
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;
	TSharedPtr<class PacketSession> GameServerSession;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AASPartyCharacterPlayer> OtherPlayerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AASCharacterNonPlayer> MonsterClass;

	AASCharacterBase* MyPlayer;

	TMap<uint64, AASCharacterBase*>Players;
};
