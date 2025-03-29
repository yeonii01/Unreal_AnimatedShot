#include "pch.h"
#include "Monster.h"

Monster::Monster()
{
	objectInfo = new Protocol::ObjectInfo();
	objectInfo->set_creature_type(Protocol::CREATURE_TYPE_MONSTER);
}

Monster::~Monster()
{
	delete objectInfo;
}