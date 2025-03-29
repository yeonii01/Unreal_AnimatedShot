#pragma once

class ObjectUtils
{
public:
	static ObjectRef CreateObject(GameSessionRef session, Protocol::CreatureType creatureType);


private:
	static atomic<int64> s_idGenerator;
};

