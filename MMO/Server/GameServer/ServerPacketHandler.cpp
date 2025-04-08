#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "Protocol.pb.h"
#include "Room.h"
#include "ObjectUtils.h"
#include "Monster.h"
#include "Player.h"
#include "GameSession.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	// TODO : DB에서 Account 정보 긁어온다
	// TODO : DB에서 유저 정보 긁어온다
	Protocol::S_LOGIN loginPkt;

	for (int32 i = 0; i < 3; i++)
	{
		Protocol::ObjectInfo* player = loginPkt.add_players();
		Protocol::PosInfo* posInfo = player->mutable_pos_info();

		posInfo->set_x(Utils::GetRandom(0.f, 100.f));
		posInfo->set_y(Utils::GetRandom(0.f, 100.f));
		posInfo->set_z(Utils::GetRandom(0.f, 100.f));
		posInfo->set_yaw(Utils::GetRandom(0.f, 45.f));
	}

	loginPkt.set_success(true);

	SEND_PACKET(loginPkt);

	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	static bool bMonsterSpawned = false;
	// 플레이어 생성
	ObjectRef object = ObjectUtils::CreateObject(static_pointer_cast<GameSession>(session), Protocol::CREATURE_TYPE_PLAYER);
	//PlayerRef player = dynamic_pointer_cast<Player>(object);
	// 방에 입장
	GRoom->DoAsync(&Room::HandleEnterObject, object);

	if (!bMonsterSpawned)
	{
		for (int i = 0; i < 11; ++i)
		{
			ObjectRef monObj = ObjectUtils::CreateObject(nullptr, Protocol::CREATURE_TYPE_MONSTER);
			MonsterRef monster = dynamic_pointer_cast<Monster>(monObj);
			auto pos = new Protocol::PosInfo();
			switch (i)
			{
			case 0:
				pos->set_x(4920.0);
				pos->set_y(7360.000016);
				pos->set_z(306.00001);
				pos->set_yaw(-90.f);
				pos->set_hp(150.f);
				pos->set_num(0);
				break;
			case 1:
				pos->set_x(5979.999897);
				pos->set_y(6967.698496);
				pos->set_z(507.698568);
				pos->set_yaw(90.f);
				pos->set_hp(150.f);
				pos->set_num(1);
				break;
			case 2:
				pos->set_x(7509.999985);
				pos->set_y(7770.0);
				pos->set_z(606.000009);
				pos->set_yaw(-90);
				pos->set_hp(150);
				pos->set_num(2);
				break;
			case 3:
				pos->set_x(8419.999985);
				pos->set_y(6969.999999);
				pos->set_z(606.00001);
				pos->set_yaw(180);
				pos->set_hp(150);
				pos->set_num(3);
				break;
			case 4:
				pos->set_x(11990.0);
				pos->set_y(9409.999984);
				pos->set_z(1606.00001);
				pos->set_yaw(-180);
				pos->set_hp(150);
				pos->set_num(4);
				break;
			case 5:
				pos->set_x(11990.0);
				pos->set_y(8989.999984);
				pos->set_z(1606.00001);
				pos->set_yaw(-180);
				pos->set_hp(150);
				pos->set_num(5);
				break;
			case 6:
				pos->set_x(17150.0);
				pos->set_y(9859.999986);
				pos->set_z(1606.000008);
				pos->set_yaw(180);
				pos->set_hp(150);
				pos->set_num(6);
				break;
			case 7:
				pos->set_x(17700.0);
				pos->set_y(9859.999986);
				pos->set_z(1606.000008);
				pos->set_yaw(180);
				pos->set_hp(150);
				pos->set_num(7);
				break;
			case 8:
				pos->set_x(17150.0);
				pos->set_y(8819.999986);
				pos->set_z(1606.000008);
				pos->set_yaw(180);
				pos->set_hp(150);
				pos->set_num(8);
				break;
			case 9:
				pos->set_x(17840.0);
				pos->set_y(8699.999986);
				pos->set_z(1606.000008);
				pos->set_yaw(180);
				pos->set_hp(150);
				pos->set_num(9);
				break;
			default:
				pos->set_x(0);
				pos->set_y(0);
				pos->set_z(0);
				pos->set_yaw(0);
				pos->set_hp(0);
				break;
			}
			monster->objectInfo->set_allocated_pos_info(pos);
			GRoom->DoAsync(&Room::HandleEnterObject, monObj);
		}
		bMonsterSpawned = true;
	}
	//GRoom->HandleEnterPlayerLocked(player);

	return true;
}

bool Handle_C_REGISTER_MONSTER(PacketSessionRef& session, Protocol::C_REGISTER_MONSTER& pkt)
{
	return true;
}

bool Handle_C_LEAVE_GAME(PacketSessionRef& session, Protocol::C_LEAVE_GAME& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->HandleLeavePlayer(player);

	return true;
}

bool Handle_C_MOVE(PacketSessionRef& session, Protocol::C_MOVE& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleMove, pkt);
	//room->HandleMove(pkt);

	return true;
}

bool Handle_C_PARTY_WEAPON(PacketSessionRef& session, Protocol::C_PARTY_WEAPON& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleRegisterWeapon,player, pkt);
	return true;
}

bool Handle_C_MONSTER_MOVE(PacketSessionRef& session, Protocol::C_MONSTER_MOVE& pkt)
{
	return true;
}

bool Handle_C_MONSTER_DAMAGEINFO(PacketSessionRef& session, Protocol::C_MONSTER_DAMAGEINFO& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleMonsterDamage, player, pkt);

	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	return true;
}
