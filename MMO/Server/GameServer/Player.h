#pragma once

class GameSession;
class Room;

class Player : public enable_shared_from_this<Player>
{
public:
	Player();
	virtual ~Player();

public:
	Protocol::PlayerInfo* playerInfo;
	weak_ptr<GameSession> session;

	//이상상태
	//vector<int32> buffs;

public:
	atomic<weak_ptr<Room>> room;
};

