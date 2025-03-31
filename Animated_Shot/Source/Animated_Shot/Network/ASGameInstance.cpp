// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/ASGameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "ClientPacketHandler.h"
#include "Character/ASCharacterPlayer.h"
#include "Character/ASCharacterNonPlayer.h"
#include "Protocol.pb.h"

UASGameInstance::UASGameInstance()
{
	OtherPlayerClass = AASPartyCharacterPlayer::StaticClass();
	MonsterClass = AASCharacterNonPlayer::StaticClass();
}

void UASGameInstance::ConnectToGameServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

		//Session
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();

		//Lobby packetsend
		{
			Protocol::C_LOGIN Pkt;
			SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
			SendPacket(SendBuffer);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void UASGameInstance::DisConnectFromGameServer()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	Protocol::C_LEAVE_GAME LeavePkt;
	SEND_PACKET(LeavePkt);
	//if (Socket)
	//{
	//	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
	//	SocketSubsystem->DestroySocket(Socket);
	//	Socket = nullptr;
	//}
}

void UASGameInstance::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->HandleRecvPackets();

}

void UASGameInstance::SendPacket(SendBufferRef SendBuffer)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->SendPacket(SendBuffer);
}

void UASGameInstance::HandleSpawn(const Protocol::ObjectInfo& ObjectInfo, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const int64 ObjectId = ObjectInfo.object_id();
	if (Players.Find(ObjectId) != nullptr)
		return;

	FVector SpawnLocation(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());
	FRotator SpawnRotation(0.0f, 90.f, 0.0f);
	FTransform SpawnTransform(SpawnRotation, SpawnLocation);
	switch (ObjectInfo.creature_type())
	{
	case Protocol::CREATURE_TYPE_PLAYER:
		if (IsMine)
		{
			auto* PC = UGameplayStatics::GetPlayerController(this, 0);
			AASCharacterPlayer* Player = Cast<AASCharacterPlayer>(PC->GetPawn());
			if (Player == nullptr) return;

			Player->SetObjectInfo(ObjectInfo.pos_info());

			MyPlayer = Player;
			Players.Add(ObjectInfo.object_id(), Player);
		}
		else
		{
			AASPartyCharacterPlayer* Player = Cast<AASPartyCharacterPlayer>(World->SpawnActor(OtherPlayerClass, &SpawnTransform));
			Player->SetObjectInfo(ObjectInfo.pos_info());
			Players.Add(ObjectInfo.object_id(), Player);
		}
		break;
	case Protocol::CREATURE_TYPE_MONSTER:
	{
		if (ObjectInfo.object_id() == 0 || Players.Contains(ObjectInfo.object_id()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Monster with ObjectId %lld already exists. Skipping spawn."), ObjectInfo.object_id());
			return;
		}
		AASCharacterNonPlayer* Monster = Cast<AASCharacterNonPlayer>(World->SpawnActor(MonsterClass, &SpawnTransform));
		if (Monster)
		{
			if (!ObjectInfo.has_pos_info())
			{
				UE_LOG(LogTemp, Error, TEXT("ObjectInfo has no pos_info!"));
			}

			Monster->SetObjectInfo(ObjectInfo.pos_info());
			Players.Add(ObjectInfo.object_id(), Monster);

			Monster->GetMesh()->SetVisibility(true);
			Monster->GetMesh()->SetHiddenInGame(false);
			Monster->SetActorHiddenInGame(false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn Monster!"));
		}
	}
		break;
	default:
		break;
	}
}

void UASGameInstance::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	HandleSpawn(EnterGamePkt.player(), true);
}

void UASGameInstance::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	for (auto& player : SpawnPkt.players())
	{
		HandleSpawn(player, false);
	}
	for (auto& monster : SpawnPkt.monsters())
	{
		HandleSpawn(monster, false);
	}
}

void UASGameInstance::HandleDespawn(uint64 ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr) return;

	//Despawn

	AASCharacterBase** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	World->DestroyActor(*FindActor);
}

void UASGameInstance::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	for (auto& ObjectId : DespawnPkt.object_ids())
	{
		HandleDespawn(ObjectId);
	}
}

void UASGameInstance::HandleMove(const Protocol::S_MOVE& MovePkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr) return;

	const uint64 ObjectId = MovePkt.info().object_id();

	AASCharacterBase** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	AASCharacterBase* Player = (*FindActor);
	if (Player->IsMyPlayer()) 
		return;

	const Protocol::PosInfo& Info = MovePkt.info();
	//Player->SetPlayerInfo(Info);
	Player->SetDestInfo(Info);
}

void UASGameInstance::HandleMonsterMove(const Protocol::S_MONSTER_MOVE& MonsterPkt)
{
	for (const Protocol::PosInfo& pos : MonsterPkt.monsters())
	{
		const int64 objectId = pos.object_id();

		AASCharacterBase** BasePtr = Players.Find(objectId);
		if (BasePtr == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[MONSTER_MOVE] objectId %lld not found in Players map!"), objectId);
			continue;
		}


		AASCharacterNonPlayer* Monster = Cast<AASCharacterNonPlayer>(*BasePtr);
		if (Monster == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[MONSTER_MOVE] objectId %lld is not a Monster!"), objectId);
			continue;
		}
		Monster->SetTargetPos(pos);
	}
}

void UASGameInstance::HandleRegisterWeapon(const Protocol::S_PARTY_WEAPON& WeaponPkt)
{
	AASCharacterBase** FindActor = Players.Find(WeaponPkt.playerid());
	if (FindActor == nullptr || *FindActor == nullptr)
		return;

	AASPartyCharacterPlayer* Player = Cast<AASPartyCharacterPlayer>(*FindActor);
	if (Player == nullptr)
		return;

	if (Player->IsMyPlayer())
		return;

	Player->EquipWeapon(WeaponPkt.weapon());
}
