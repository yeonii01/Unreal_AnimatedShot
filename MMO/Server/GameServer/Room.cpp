#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"
#include "Monster.h"
#include "ObjectUtils.h"

RoomRef GRoom = make_shared<Room>();

Room::Room()
{

}

Room::~Room()
{

}

bool Room::EnterRoom(ObjectRef object, bool randPos/* = true*/)
{
	bool success = AddObject(object);

	// 랜덤 위치
	if (randPos)
	{
		object->posInfo->set_x(Utils::GetRandom(2900.f, 3100.f));
		object->posInfo->set_y(4480.f);
		object->posInfo->set_z(108.f);
		object->posInfo->set_yaw(90.f);
		object->posInfo->set_hp(150.f);
	}

	// 입장 사실을 신입 플레이어에게 알린다
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(success);

		Protocol::ObjectInfo* playerInfo = new Protocol::ObjectInfo();
		playerInfo->CopyFrom(*object->objectInfo);
		enterGamePkt.set_allocated_player(playerInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 입장 사실을 다른 플레이어에게 알린다
	{
		Protocol::S_SPAWN spawnPkt;

		if (object->IsPlayer())
		{
			Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
			objectInfo->CopyFrom(*object->objectInfo);
		}
		else
		{
			Protocol::ObjectInfo* objectInfo = spawnPkt.add_monsters();
			objectInfo->CopyFrom(*object->objectInfo);
		}
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, object->objectInfo->object_id());
	}

	// 기존 입장한 플레이어 목록을 신입 플레이어한테 전송해준다
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& item : _objects)
		{

			if (item.second->IsPlayer())
			{
				Protocol::ObjectInfo* playerInfo = spawnPkt.add_players();
				playerInfo->CopyFrom(*item.second->objectInfo);
			}
			else
			{
				Protocol::ObjectInfo* monsterInfo = spawnPkt.add_monsters();
				monsterInfo->CopyFrom(*item.second->objectInfo); 
			}

		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	return success;
}

bool Room::LeaveRoom(ObjectRef object)
{
	if (object == nullptr)
		return false;

	const uint64 objectId = object->objectInfo->object_id();
	bool success = RemoveObject(objectId);

	// 퇴장 사실을 퇴장하는 플레이어에게 알린다
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 퇴장 사실을 알린다
	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_object_ids(objectId);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(despawnPkt);
		Broadcast(sendBuffer, objectId);

		if (auto player = dynamic_pointer_cast<Player>(object))
			if (auto session = player->session.lock())
				session->Send(sendBuffer);
	}

	return success;
}

bool Room::HandleEnterObject(ObjectRef object)
{
	if(object->IsPlayer())
		return EnterRoom(object, true);
	return EnterRoom(object, false);
}

bool Room::HandleLeavePlayer(PlayerRef player)
{
	return LeaveRoom(player);
}

void Room::HandleMove(Protocol::C_MOVE pkt)
{
	const uint64 objectId = pkt.info().object_id();
	if (_objects.find(objectId) == _objects.end())
		return;

	// 적용
	PlayerRef player = dynamic_pointer_cast<Player>(_objects[objectId]);
	player->posInfo->CopyFrom(pkt.info());

	{
		Protocol::S_MOVE movePkt;
		{
			Protocol::PosInfo* info = movePkt.mutable_info();
			info->CopyFrom(pkt.info());
		}
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
		Broadcast(sendBuffer);
	}
}

void Room::HandleRegisterWeapon(PlayerRef player, Protocol::C_PARTY_WEAPON pkt)
{
	if (player == nullptr)
		return;

	Protocol::S_PARTY_WEAPON weaponPkt;
	{
		weaponPkt.set_playerid(player->posInfo->object_id());
		weaponPkt.set_weapon(pkt.weapon());
	}
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(weaponPkt);
	Broadcast(sendBuffer);
}

void Room::HandleMonsterDamage(PlayerRef player, Protocol::C_MONSTER_DAMAGEINFO pkt)
{
	Protocol::S_MONSTER_DAMAGEINFO damagePkt;
	{
		Protocol::PosInfo* info = damagePkt.mutable_monsters();
		info->CopyFrom(pkt.monsters());
	}
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(damagePkt);
	Broadcast(sendBuffer, player->posInfo->object_id());
}

void Room::UpdateTick()
{
	_time += 100;

	Protocol::S_SERVER_TIME timepkt;
	{
		timepkt.set_server_time_ms(_time);
	}
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(timepkt);
	Broadcast(sendBuffer);

	if (_time % 3000 == 0)
	{
		Protocol::S_MONSTER_MOVE monsterpkt;
		for (auto& item : _objects)
		{
			if (item.second->objectInfo == nullptr || item.second->objectInfo->has_pos_info() == false)
				continue;

			MonsterRef monster = dynamic_pointer_cast<Monster>(item.second);
			if (monster == nullptr)
				continue;

			Protocol::PosInfo* monsterInfo = monsterpkt.add_monsters();
			monsterInfo->CopyFrom(item.second->objectInfo->pos_info());
			float offsetX = Utils::GetRandom(-500.f, 500.f);
			float offsetY = Utils::GetRandom(-500.f, 500.f);

			monsterInfo->set_object_id(item.second->objectInfo->object_id());
			monsterInfo->set_x(monsterInfo->x() + offsetX);
			monsterInfo->set_y(monsterInfo->y() + offsetY);
			monsterInfo->set_z(monsterInfo->z());
			//cout << monsterInfo->x() << ", " << monsterInfo->y() << ", " << monsterInfo->z() << endl;
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(monsterpkt);
		Broadcast(sendBuffer);
	}
	DoTimer(100, &Room::UpdateTick);
}

RoomRef Room::GetRoomRef()
{
	return static_pointer_cast<Room>(shared_from_this());
}

bool Room::AddObject(ObjectRef object)
{
	// 있다면 문제가 있다.
	if (_objects.find(object->objectInfo->object_id()) != _objects.end())
		return false;

	_objects.insert(make_pair(object->objectInfo->object_id(), object));

	object->room.store(GetRoomRef());

	return true;
}

bool Room::RemoveObject(uint64 objectId)
{
	// 없다면 문제가 있다.
	if (_objects.find(objectId) == _objects.end())
		return false;

	ObjectRef object = _objects[objectId];
	PlayerRef player = dynamic_pointer_cast<Player>(object);
	if (player)
		player->room.store(weak_ptr<Room>());

	_objects.erase(objectId);

	return true;
}

void Room::Broadcast(SendBufferRef sendBuffer, uint64 exceptId)
{
	for (auto& item : _objects)
	{
		PlayerRef player = dynamic_pointer_cast<Player>(item.second);
		if (player == nullptr)
			continue;
		if (player->objectInfo->object_id() == exceptId)
			continue;

		if (GameSessionRef session = player->session.lock())
			session->Send(sendBuffer);
	}
}