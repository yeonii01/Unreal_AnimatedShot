#include "pch.h"
#include "ObjectUtils.h"
#include "Monster.h"
#include "Player.h"
#include "GameSession.h"

atomic<int64> ObjectUtils::s_idGenerator = 1;

ObjectRef ObjectUtils::CreateObject(GameSessionRef session, Protocol::CreatureType creatureType)
{
	// ID »ý¼º±â
	const int64 newId = s_idGenerator.fetch_add(1);

	ObjectRef object;
	switch (creatureType)
	{
	case Protocol::CREATURE_TYPE_PLAYER:
		object = make_shared<Player>();
		break;
	case Protocol::CREATURE_TYPE_MONSTER:
		object = make_shared<Monster>();
		break;
	default:
		object = make_shared<Object>();
		break;
	}
	object->objectInfo->set_object_type(Protocol::OBJECT_TYPE_CREATURE);
	object->objectInfo->set_object_id(newId);
	object->posInfo->set_object_id(newId);

	if (object->IsPlayer())
	{
		PlayerRef player = dynamic_pointer_cast<Player>(object);
		player->session = session;
		session->player.store(player);
	}
	return object;
}