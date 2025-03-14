// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Animated_Shot.h"
#include "ASGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATED_SHOT_API UASGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void DisConnectFromGameServer();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);

public:
	//GameServer
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;
	TSharedPtr<class PacketSession> GameServerSession;


};
