#include "pch.h"
#include "Player.h"

Player::Player()
{
	_isPlayer = true;
	objectInfo->set_creature_type(Protocol::CREATURE_TYPE_PLAYER);
}

Player::~Player()
{

}
